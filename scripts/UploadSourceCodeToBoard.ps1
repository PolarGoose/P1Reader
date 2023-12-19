param (
  [Parameter(Mandatory)][string] $comPort,
  [Parameter(Mandatory)][string] $ssid,
  [Parameter(Mandatory)][string] $pass)

Import-Module "$PSScriptRoot/Common.psm1" -Force

RemoveAllFilesFromTheBoard -comPort $comPort
UploadSourceCodeToTheBoard -comPort $comPort -ssid $ssid -pass $pass
