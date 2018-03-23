
param($port = "COM12", $baud = 115200, [Parameter(Mandatory=$True)]$file)

$delay = 100
$offset = 40
$port = new-Object System.IO.Ports.SerialPort $port,$baud,None,8,one

# Read source file
$basename = Split-Path -Path $file -Leaf -Resolve
$content = [System.IO.File]::ReadAllText( ( Resolve-Path $file ) )
$Bytes = [System.Text.Encoding]::UTF8.GetBytes($content)
$EncodedText =[Convert]::ToBase64String($Bytes)
$n = $EncodedText.length
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
$port.WriteLine( "f.write(ubinascii.a2b_base64(b'")
start-sleep -m $delay
Write-Host $port.ReadExisting()
while( ($i+$offset) -lt $n )
{
    $port.Write($EncodedText,$i,$offset)
    $i += $offset
    start-sleep -m $delay
    Write-Host $port.ReadExisting()
}
$port.Write($EncodedText,$i,$n-$i)
start-sleep -m $delay
Write-Host $port.ReadExisting()
$port.WriteLine(  "'))" + "`r" )
start-sleep -m $delay
Write-Host $port.ReadExisting()
$port.WriteLine( "f.close()" + "`r" )
start-sleep -m $delay
Write-Host $port.ReadExisting()
$port.Close() 