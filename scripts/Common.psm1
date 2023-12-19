Set-StrictMode -Version Latest
$global:ErrorActionPreference = "Stop"
$global:ProgressPreference = "SilentlyContinue"

$root = Resolve-Path "$PSScriptRoot/.."
$buildDir = "$root/build"
$toolsDir = "$buildDir/tools"
$tempDir = "$buildDir/temp"
$srcDir = "$root/src/app"
$resDir = "$root/res"
$libDir = "$buildDir/lib"
$logsDir = "$buildDir/logs"
Add-Type -AssemblyName System.IO.Compression.FileSystem

Function Info($msg) {
  Write-Host -ForegroundColor DarkGreen "`nINFO: $msg`n"
}

Function Error($msg) {
  Write-Host `n`n
  Write-Error $msg
  exit 1
}

Function CheckReturnCodeOfPreviousCommand($msg) {
  if(-Not $?) {
    Error "${msg}. Error code: $LastExitCode"
  }
}

Function DownloadAndExtract([string] $url, [System.IO.FileInfo] $outputFile) {
  Invoke-WebRequest -Uri $url -OutFile $outputFile

  if($outputFile.Extension -eq ".zip") {
    [System.IO.Compression.ZipFile]::ExtractToDirectory($outputFile, $( Split-Path $outputFile ))
    return
  }

  if($outputFile -like "*.tar.gz") {
    tar --extract --verbose --gzip --file $outputFile --directory $(Split-Path $outputFile)
    return
  }
}

Function CreateDirectory([System.IO.DirectoryInfo] $dir) {
  New-Item $dir -Force -ItemType "directory" > $null
}

Function RemoveDirectory([System.IO.DirectoryInfo] $dir) {
  Remove-Item $dir -Force -Recurse -ErrorAction SilentlyContinue
}

Function DownloadTools() {
  CreateDirectory $toolsDir
  RemoveDirectory $tempDir
  CreateDirectory $tempDir

  if(! (Test-Path -Path "$toolsDir/esptool.exe"))
  {
    Info "Download EspTool"
    DownloadAndExtract `
      -url "https://github.com/espressif/esptool/releases/download/v4.7.0/esptool-v4.7.0-win64.zip" `
      -outputFile "$tempDir/esptool.zip"

    Copy-Item -Path "$tempDir/esptool-win64/esptool.exe" -Destination "$toolsDir" -Force
  }

  if(! (Test-Path -Path "$toolsDir/esp32MicropythonFW.bin"))
  {
    Info "Download Micropython FW"
    Invoke-WebRequest `
      -Uri "https://micropython.org/resources/firmware/ESP32_GENERIC-20240602-v1.23.0.bin" `
      -OutFile "$toolsDir/esp32MicropythonFW.bin"
  }

  if(! (Test-Path -Path "$toolsDir/pyboard.py"))
  {
    Info "Download pyboard.py"
    Invoke-WebRequest `
      -Uri "https://raw.githubusercontent.com/micropython/micropython/master/tools/pyboard.py" `
      -OutFile "$toolsDir/pyboard.py"
  }

  if(! (Test-Path -Path "$toolsDir/python"))
  {
    Info "Download Python"
    Invoke-WebRequest `
      -Uri "https://www.python.org/ftp/python/3.12.4/python-3.12.4-embed-amd64.zip" `
      -OutFile "$tempDir/python.zip"
    CreateDirectory "$toolsDir/python"
    [System.IO.Compression.ZipFile]::ExtractToDirectory("$tempDir/python.zip", "$toolsDir/python")
  }

  if(! (Test-Path -Path "$toolsDir/python/serial"))
  {
    Info "Download pyserial"
    DownloadAndExtract `
      -url "https://files.pythonhosted.org/packages/07/bc/587a445451b253b285629263eb51c2d8e9bcea4fc97826266d186f96f558/pyserial-3.5-py2.py3-none-any.whl" `
      -outputFile "$tempDir/pyserial.zip"
    Copy-Item -Path "$tempDir/serial" -Destination "$toolsDir/python" -Force -Recurse
  }

  if(! (Test-Path -Path "$toolsDir/python/mpremote"))
  {
    Info "Download mpremote"
    DownloadAndExtract `
      -url "https://files.pythonhosted.org/packages/63/d9/f8356a5fea06331e1a82f186cdf01dd588f705a88d6ffce40f0c6cadc7b0/mpremote-1.23.0-py3-none-any.whl" `
      -outputFile "$tempDir/mpremote.zip"
    Copy-Item -Path "$tempDir/mpremote" -Destination "$toolsDir/python" -Force -Recurse
  }

  if(! (Test-Path -Path "$toolsDir/plink.exe"))
  {
    Info "Download plink"
    Invoke-WebRequest `
      -Uri "https://the.earth.li/~sgtatham/putty/latest/w64/plink.exe" `
      -OutFile "$toolsDir/plink.exe"
  }

  if(! (Test-Path -Path "$libDir/logging.py"))
  {
    Info "Download Micropython package: logging"
    Invoke-WebRequest `
      -Uri "https://raw.githubusercontent.com/micropython/micropython-lib/master/python-stdlib/logging/logging.py" `
      -OutFile "$libDir/logging.py"
  }

  if(! (Test-Path -Path "$libDir/tinyweb"))
  {
    Info "Download Micropython package: tinyweb"
    DownloadAndExtract `
      -url "https://github.com/belyalov/tinyweb/archive/refs/heads/master.zip" `
      -outputFile "$tempDir/tinyweb.zip"
    Copy-Item -Path "$tempDir/tinyweb-master/tinyweb" -Destination "$libDir/tinyweb" -Force -Recurse
  }
}

Function UploadMicropythonFirmwareToTheBoard([string] $comPort) {
  Info "Erase flash"
  & "$toolsDir/esptool.exe" --chip esp32 --port $comPort erase_flash
  CheckReturnCodeOfPreviousCommand "Failed to erase flash"

  Info "Upload FW to the board: $toolsDir/esp32MicropythonFW.bin"
  & "$toolsDir/esptool.exe" --chip esp32 --port $comPort --baud 1500000 write_flash -z 0x1000 "$toolsDir/esp32MicropythonFW.bin"
  CheckReturnCodeOfPreviousCommand "Failed to upload FW"
}

Function RunMpRemote() {
  $python = Get-Command -Name "$toolsDir/python/python.exe"
  & $python -m mpremote $args
}

Function CopyFileToBoard([string] $comPort, [System.IO.DirectoryInfo] $directory, [string] $file) {
  Info "Copy to board: $directory/$file"
  push-location $directory
  try {
    $recursiveFlag = ""
    if(Test-Path -Path $file -PathType Container) {
        $recursiveFlag = "-r"
    }
    RunMpRemote connect $comPort fs cp $recursiveFlag $file ":"
    CheckReturnCodeOfPreviousCommand "Failed to upload"
  }
  finally
  {
    Pop-Location
  }
}

Function RemoveAllFilesFromTheBoard([string] $comPort) {
  Info "Remove all files from the board"
  RunMpRemote connect $comPort run "$resDir/remove_all_files_from_the_board.py"
}

Function UploadSourceCodeToTheBoard([string] $comPort, [string] $ssid, [string] $pass) {
  Info "Upload source code to the board"
  foreach($fileOrFolder in Get-ChildItem $srcDir) {
    CopyFileToBoard $comPort $srcDir $fileOrFolder
  }
  CopyFileToBoard $comPort $buildDir lib

  Set-Content -Path "$tempDir/wifi_credentials.txt" -Value "$ssid`n$pass"
  CopyFileToBoard $comPort $tempDir wifi_credentials.txt
}

Function RestartBoard([string] $comPort) {
  Info "Restart board"
  & "$toolsDir/esptool.exe" --chip esp32 --port $comPort read_mac
}

Function OpenRepl([string] $comPort) {
  Info "Open REPL"
  & "$toolsDir/plink.exe" -serial $comPort -sercfg 115200,8,1,N,N
}

Export-ModuleMember -Function *
