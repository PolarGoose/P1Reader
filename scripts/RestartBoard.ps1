param ([Parameter(Mandatory)][string] $comPort)

Import-Module "$PSScriptRoot/Common.psm1" -Force

RestartBoard -comPort $comPort
