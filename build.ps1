[CmdletBinding()]
param(
    [ValidateSet('Release', 'Debug')]
    [string]$Configuration = 'Release',

    [string]$QtRoot,

    [ValidateRange(1, 64)]
    [int]$Jobs = 4,

    [switch]$Run
)

$ErrorActionPreference = 'Stop'

if (-not $env:CONDA_PREFIX) {
    throw 'Activate the ultrasonic-hmi Conda environment before building.'
}

$fftwDll = Join-Path $env:CONDA_PREFIX 'Library\bin\fftw3.dll'
if (-not (Test-Path -LiteralPath $fftwDll -PathType Leaf)) {
    throw 'FFTW is missing from the active Conda environment. Create or update it from environment.yml.'
}

if (-not $QtRoot) {
    $QtRoot = Join-Path $env:CONDA_PREFIX 'qt-sdk'
}

$QtRoot = $ExecutionContext.SessionState.Path.GetUnresolvedProviderPathFromPSPath($QtRoot)
$qtBin = Join-Path $QtRoot '5.15.2\mingw81_64\bin'
$mingwBin = Join-Path $QtRoot 'Tools\mingw810_64\bin'
$qmake = Join-Path $qtBin 'qmake.exe'
$make = Join-Path $mingwBin 'mingw32-make.exe'
if (-not (Test-Path -LiteralPath $qmake -PathType Leaf) -or
    -not (Test-Path -LiteralPath $make -PathType Leaf)) {
    throw "Qt 5.15.2 and MinGW 8.1 were not found under '$QtRoot'. See README.md for installation."
}

$variant = $Configuration.ToLowerInvariant()
$project = '..\..\UltraSonicHMI\UltraSonicHMI.pro'
$buildDir = Join-Path $PSScriptRoot "build\$variant"
New-Item -ItemType Directory -Path $buildDir -Force | Out-Null

$envPath = $env:Path
$compilerVariables = @(
    'CPATH', 'C_INCLUDE_PATH', 'CPLUS_INCLUDE_PATH', 'LIBRARY_PATH',
    'COMPILER_PATH', 'GCC_EXEC_PREFIX'
)
$compilerEnvironment = @{}
$env:Path = "$mingwBin;$qtBin;$(Split-Path -Parent $fftwDll);$envPath"
Push-Location -LiteralPath $buildDir
try {
    foreach ($name in $compilerVariables) {
        $compilerEnvironment[$name] = [Environment]::GetEnvironmentVariable($name)
        [Environment]::SetEnvironmentVariable($name, $null)
    }

    & $qmake -spec win32-g++ $project 'CONFIG-=debug_and_release' 'CONFIG-=debug_and_release_target' "CONFIG+=$variant"
    if ($LASTEXITCODE -ne 0) {
        throw "qmake failed (exit $LASTEXITCODE)."
    }

    & $make "SHELL=$env:ComSpec" "-j$Jobs"
    if ($LASTEXITCODE -ne 0) {
        throw "Build failed (exit $LASTEXITCODE)."
    }

    $executable = Join-Path $buildDir 'UltraSonicHMI.exe'
    if (-not (Test-Path -LiteralPath $executable -PathType Leaf)) {
        throw "Build output is missing: $executable"
    }
    Write-Host "Built $executable"

    if ($Run) {
        $process = Start-Process -FilePath $executable -WorkingDirectory $buildDir -Wait -PassThru
        if ($process.ExitCode -ne 0) {
            throw "UltraSonicHMI exited with code $($process.ExitCode)."
        }
    }
} finally {
    Pop-Location
    $env:Path = $envPath
    foreach ($name in $compilerEnvironment.Keys) {
        [Environment]::SetEnvironmentVariable($name, $compilerEnvironment[$name])
    }
}
