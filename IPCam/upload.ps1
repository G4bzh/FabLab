param($port = "COM12", $baud = 115200, $path, $retry = "3", $buffer=50, [Parameter(Mandatory=$True)]$file)

# Helper : flush serial
function flushSerial
{
    param( [Parameter(Mandatory=$True)]$sp, $d = 200 )

    start-sleep -m $d
    $sp.ReadExisting() | out-null
}

# Helper : print serial
function checkSerial
{
    param( [Parameter(Mandatory=$True)]$sp, [Parameter(Mandatory=$True)]$file, $prefix="        " )

    # Check prompt
    $sp.WriteLine( "`r" )
    start-sleep -m 200
    $prompt = $sp.ReadExisting()
    if ( $prompt.Substring($prompt.Length - 4) -ne ">>> " )
    {
        Write-Host "$($prefix)[x] Failure : no prompt"
        Write-Host "$($prefix)[x] Resetting board ..."
        $serial.Write([char]4)
        flushSerial -sp $serial -d 1500
        return 0
    }

    # Compute source file SHA1
    $sha1 = New-Object System.Security.Cryptography.SHA1CryptoServiceProvider
    $digest = [System.BitConverter]::ToString( $sha1.ComputeHash([System.IO.File]::ReadAllBytes(( Resolve-Path $file ))))
    $digest = $digest -replace '-',''


    # Compute remote file SHA1
    $basename = Split-Path -Path $file -Leaf -Resolve
    $sp.WriteLine( "import uhashlib" + "`r")
    flushSerial -sp $sp

    $sp.WriteLine( "ubinascii.hexlify(uhashlib.sha1(open('" + $basename + "').read()).digest()).upper()" + "`r")
    start-sleep -m 200
    $sp.ReadLine() | out-null
    $rdigest = $sp.ReadLine().split("'")[1]

    if ( $rdigest -ne $digest )
    {
        write-Host "$($prefix)[x] Failure : wrong SHA1 digest"
        Write-Host "$($prefix)[x] Resetting board ..."
        $serial.Write([char]4)
        flushSerial -sp $serial -d 1500
        return 0
    }


    Write-Host "$($prefix)[.] Success "
    return 1
}


# Create serial object
$serial = new-Object System.IO.Ports.SerialPort $port,$baud,None,8,one
Write-Host "[.] Connecting to $($port) at $($baud)"

# Read source file and convert to base64
$basename = Split-Path -Path $file -Leaf -Resolve
$content = [System.IO.File]::ReadAllBytes( ( Resolve-Path $file ) )
$Bytes = $content | ForEach-Object ToString X2


# Retry counter
$r=0


# Open and write destination File
Try
{
    $serial.Open()

    Write-Host "[.] Uploading $($file)"
    while( $r -lt $retry)
    {

        Write-Host "    [.] try $($r+1)/$($retry)..." 

        $serial.WriteLine( "`r" )
        $serial.WriteLine( "import os" + "`r" )
        flushSerial -sp $serial
        
        $serial.WriteLine( "import ubinascii" + "`r" )
        flushSerial -sp $serial

        # Create and move to path
        if ($path)
        {
            Write-Host "    [.] Moving to $($path)"
            $serial.WriteLine( "os.chdir('/')" + "`r" )
            flushSerial -sp $serial
            foreach ($dir in $path.split("/\"))
            {
                if ($dir)
                {
                    $serial.WriteLine( "os.mkdir('" + $dir + "')" + "`r" )
                    flushSerial -sp $serial
                    $serial.WriteLine( "os.chdir('" + $dir + "')" + "`r" )
                    flushSerial -sp $serial
                }
            }
        }

        $serial.WriteLine( "f = open('" + $basename +"', 'wb')" + "`r" )
        flushSerial -sp $serial

        # Char counter
        $i=0
        $offset = $buffer

        # Send file byte by byte
        while( $i -lt $Bytes.length)
        {
                    
            if ($i + $offset -gt $Bytes.length)
            {
                $offset = $Bytes.length - $i
            }

            $s=""
            for ($j=0; $j -lt $offset ; $j ++)
            {
                $s+="\x$($Bytes[$i+$j])"
            }

            $serial.WriteLine( "f.write(b'" + $s +"')" + "`r" )
            flushSerial -sp $serial

            $i+=$offset
            $a = [int]($i*100/$Bytes.length)
            Write-Progress -Activity "Uploading $($basename)..." -PercentComplete $a -CurrentOperation "$a% complete" -Status "Please wait."
            
        }
        flushSerial -sp $serial

        $serial.WriteLine( "f.close()" + "`r" )
        flushSerial -sp $serial

        
        Write-Host "    [.] Upload finished"
        Write-Host "    [.] Checking board integrity ..."
        
        if ( (checkSerial -sp $serial -file $file) -eq 1 )
        {
            break
        }

        # Back to root if necessary
        if ($path)
        {
            $serial.WriteLine( "os.chdir('/')" + "`r" )
            flushSerial -sp $serial
        }

        $r+=1
    }

    # Back to root if necessary
    if ($path)
    {
        $serial.WriteLine( "os.chdir('/')" + "`r" )
        flushSerial -sp $serial
    }

    $serial.Close() 
    Write-Host "[.] Bye"
}
Catch
{
    $ErrorMessage = $_.Exception.Message
    Write-Host "[x] Error : " $ErrorMessage
}