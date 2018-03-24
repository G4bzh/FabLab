param($port = "COM12", $baud = 115200, [Parameter(Mandatory=$True)]$file)

# Helper : flush serial
function flushSerial
{
    param( [Parameter(Mandatory=$True)]$sp )

    start-sleep -m 200
    $sp.ReadExisting() | out-null
}

# Helper : print serial
function checkSerial
{
    param( [Parameter(Mandatory=$True)]$sp, $prefix="    " )

    start-sleep -m 200
    $prompt = $sp.ReadExisting()
    if ( $prompt.Substring($prompt.Length - 4) -eq ">>> " )
    {
        Write-Host "$($prefix)[.] Success"
    }
    else 
    {
        Write-Host "$($prefix)[x] Failure"
    }
    
}


# Create serial object
$serial = new-Object System.IO.Ports.SerialPort $port,$baud,None,8,one
Write-Host "[.] Connecting to $($port) at $($baud)"


# Read source file and convert to base64
$basename = Split-Path -Path $file -Leaf -Resolve
$content = [System.IO.File]::ReadAllText( ( Resolve-Path $file ) )
$Bytes = [System.Text.Encoding]::UTF8.GetBytes($content)
$EncodedText =[Convert]::ToBase64String($Bytes)

$i=0

# Open and write destination File
Try
{
    $serial.Open()
    Write-Host "[.] Uploading $($file) ..."
    
    $serial.WriteLine( "`r" )
    $serial.WriteLine( "import os" + "`r" )
    flushSerial -sp $serial
    
    $serial.WriteLine( "import ubinascii" + "`r" )
    flushSerial -sp $serial
    
    $serial.WriteLine( "f = open('" + $basename +"', 'wb')" + "`r" )
    flushSerial -sp $serial

    $pad = "f.write(ubinascii.a2b_base64(b'" + $EncodedText + "'))" + "`r"
    while( $i -lt $pad.length)
    {
        $serial.Write($pad,$i,1)
        $a = [int]($i*100/$pad.length)
        Write-Progress -Activity "Uploading $($basename)..." -PercentComplete $a -CurrentOperation "$a% complete" -Status "Please wait."
        $i+=1
        start-sleep -m 5
    }
    flushSerial -sp $serial

    $serial.WriteLine( "f.close()" + "`r" )
    flushSerial -sp $serial
    
    Write-Host "[.] Upload finished"
    Write-Host "[.] Checking board integrity ..."
    
    $serial.WriteLine( "`r" )
    checkSerial -sp $serial 

    $serial.Close() 
    Write-Host "[.] Bye"
}
Catch
{
    $ErrorMessage = $_.Exception.Message
    Write-Host "[x] Error : " $ErrorMessage
}