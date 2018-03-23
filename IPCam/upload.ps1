
param($port = "COM12", $baud = 115200, [Parameter(Mandatory=$True)]$file)


$port = new-Object System.IO.Ports.SerialPort $port,$baud,None,8,one

# Read source file
$file = Get-Childitem -path $file
$basename = Split-Path -Path $file -Leaf -Resolve

# Open and write destination File
$port.Open()
$port.WriteLine( "`r" )
$port.WriteLine( "import os" + "`r" )
$port.WriteLine( "f = open('" + $basename +"', 'w')" + "`r" )
foreach( $line in [System.IO.File]::ReadLines($file))
{
    $port.WriteLine( "f.write('" + $line + "\n')" + "`r" )
}
$port.WriteLine( "f.close()" + "`r" )
start-sleep -m 50
$port.Close()