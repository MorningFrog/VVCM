$ErrorActionPreference = "Stop"
$ProgressPreference = "SilentlyContinue"

# install vcpkg
$env:VCPKG_ROOT = "C:\vcpkg"
if (-not (Test-Path $env:VCPKG_ROOT)) {
  git clone https://github.com/microsoft/vcpkg.git $env:VCPKG_ROOT
}

# Clear and backup existing CMake environment variables
$backupVars = @{}
foreach ($var in @("CMAKE_TOOLCHAIN_FILE", "CMAKE_PREFIX_PATH", "CMAKE_MODULE_PATH")) {
    if (Test-Path "Env:$var") {
        $backupVars[$var] = (Get-Item "Env:$var").Value
        Remove-Item "Env:$var"
    }
}

& "$env:VCPKG_ROOT\bootstrap-vcpkg.bat" -disableMetrics

# Restore backed up CMake environment variables
foreach ($kv in $backupVars.GetEnumerator()) {
    Set-Item "Env:$($kv.Key)" $kv.Value
}

# install eigen3
& "$env:VCPKG_ROOT\vcpkg" install "eigen3:$env:VCPKG_TARGET_TRIPLET"