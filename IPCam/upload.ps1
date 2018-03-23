
param($port = "COM12", $baud = 115200, [Parameter(Mandatory=$True)]$file)

$delay = 200
$port = new-Object System.IO.Ports.SerialPort $port,$baud,None,8,one

# Read source file
$basename = Split-Path -Path $file -Leaf -Resolve
$content = [System.IO.File]::ReadAllText( ( Resolve-Path $file ) )
$Bytes = [System.Text.Encoding]::UTF8.GetBytes($content)
$EncodedText =[Convert]::ToBase64String($Bytes)

$i=0

# Open and write destination File
$port.Open()
$port.WriteLine( "`r" )
$port.WriteLine( "import os" + "`r" )
start-sleep -m $delay
Write-Host $port.ReadExisting()
$port.WriteLine( "import ubinascii" + "`r" )
start-sleep -m $delay
Write-Host $port.ReadExisting()
$port.WriteLine( "f = open('" + $basename +"', 'wb')" + "`r" )
start-sleep -m $delay
Write-Host $port.ReadExisting()

$pad = "f.write(ubinascii.a2b_base64(b'" + $EncodedText + "'))" + "`r"
while( $i -lt $pad.length)
{
    $port.Write($pad,$i,1)
    $i+=1
    start-sleep -m 5
}
Write-Host $port.ReadExisting()

$port.WriteLine( "f.close()" + "`r" )
start-sleep -m $delay
Write-Host $port.ReadExisting()
$port.Close() 