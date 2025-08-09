$ErrorActionPreference = "Stop"
$ProgressPreference = "SilentlyContinue"

# install vcpkg
$env:VCPKG_ROOT = "C:\vcpkg"
if (-not (Test-Path $env:VCPKG_ROOT)) {
  git clone https://github.com/microsoft/vcpkg.git $env:VCPKG_ROOT
}
& "$env:VCPKG_ROOT\bootstrap-vcpkg.bat" -disableMetrics | Out-Host

# install eigen3
& "$env:VCPKG_ROOT\vcpkg" install "Eigen3:x64-windows"