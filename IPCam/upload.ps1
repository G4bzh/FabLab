
param($port = "COM12", $baud = 115200, [Parameter(Mandatory=$True)]$file)


$port = new-Object System.IO.Ports.SerialPort $port,$baud,None,8,one

# Read source file
$basename = Split-Path -Path $file -Leaf -Resolve

$content = [System.IO.File]::ReadAllText( ( Resolve-Path $file ) )
$Bytes = [System.Text.Encoding]::UTF8.GetBytes($content)
$EncodedText =[Convert]::ToBase64String($Bytes)

# Open and write destination File
$port.Open()
$port.WriteLine( "`r" )
$port.WriteLine( "import os" + "`r" )
$port.WriteLine( "import ubinascii" + "`r" )
$port.WriteLine( "f = open('" + $basename +"', 'wb')" + "`r" )
$port.WriteLine( "f.write(ubinascii.a2b_base64(b'" + $EncodedText + "'))" + "`r" )
$port.WriteLine( "f.close()" + "`r" )
start-sleep -m 50
$port.Close()