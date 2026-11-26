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

function Copy-RuntimeFile([string]$Source, [string]$Destination) {
    $file = Get-Item -LiteralPath $Source
    $current = Get-Item -LiteralPath $Destination -ErrorAction SilentlyContinue
    if (-not $current -or $file.Length -ne $current.Length -or
        $file.LastWriteTimeUtc -ne $current.LastWriteTimeUtc) {
        Copy-Item -LiteralPath $file.FullName -Destination $Destination -Force
    }
}

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
    $qtLibraries = 'Qt5Core.dll', 'Qt5Gui.dll', 'Qt5Network.dll',
        'Qt5PrintSupport.dll', 'Qt5SerialPort.dll', 'Qt5Widgets.dll'
    foreach ($library in $qtLibraries) {
        Copy-RuntimeFile (Join-Path $qtBin $library) (Join-Path $buildDir $library)
    }

    $mingwLibraries = 'libgcc_s_seh-1.dll', 'libstdc++-6.dll', 'libwinpthread-1.dll'
    foreach ($library in $mingwLibraries) {
        Copy-RuntimeFile (Join-Path $mingwBin $library) (Join-Path $buildDir $library)
    }

    Copy-RuntimeFile $fftwDll (Join-Path $buildDir 'fftw3.dll')
    $vcRuntime = Join-Path (Split-Path -Parent $fftwDll) 'vcruntime140.dll'
    Copy-RuntimeFile $vcRuntime (Join-Path $buildDir 'vcruntime140.dll')

    $qtPlugins = Join-Path (Split-Path -Parent $qtBin) 'plugins'
    foreach ($plugin in 'platforms\qwindows.dll', 'printsupport\windowsprintersupport.dll',
                        'styles\qwindowsvistastyle.dll') {
        $destination = Join-Path $buildDir $plugin
        New-Item -ItemType Directory -Path (Split-Path -Parent $destination) -Force | Out-Null
        Copy-RuntimeFile (Join-Path $qtPlugins $plugin) $destination
    }

    $qtConfig = Join-Path $buildDir 'qt.conf'
    Set-Content -LiteralPath $qtConfig -Encoding ascii -Value '[Paths]', 'Plugins=.'

    $example = Join-Path $PSScriptRoot 'examples\ultrasonic-demo.hex'
    Copy-RuntimeFile $example (Join-Path $buildDir 'ultrasonic-demo.hex')
    Write-Host "Built $executable"

    if ($Run) {
        $process = Start-Process -FilePath $executable -WorkingDirectory $buildDir -Wait -PassThru
        if ($process.ExitCode -ne 0) {
            throw ('UltraSonicHMI exited with code 0x{0:X8}.' -f $process.ExitCode)
        }
    }
} finally {
    Pop-Location
    $env:Path = $envPath
    foreach ($name in $compilerEnvironment.Keys) {
        [Environment]::SetEnvironmentVariable($name, $compilerEnvironment[$name])
    }
}
