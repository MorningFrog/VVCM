#################################
# Install vcpkg script, Windows #
#################################

$ErrorActionPreference = "Stop"

# Set installation path (modifiable)
$vcpkgDir = "$env:USERPROFILE\vcpkg"

Write-Host "Installing vcpkg to $vcpkgDir"

# Clone vcpkg
if (-Not (Test-Path $vcpkgDir)) {
    git clone https://github.com/microsoft/vcpkg.git $vcpkgDir
}

# Bootstrap the installation
Set-Location $vcpkgDir
.\bootstrap-vcpkg.bat

# Optional: Add to system PATH (current user)
$envPath = [Environment]::GetEnvironmentVariable("Path", "User")
if ($envPath -notlike "*$vcpkgDir*") {
    [Environment]::SetEnvironmentVariable("Path", "$envPath;$vcpkgDir", "User")
    Write-Host "Added vcpkg to PATH (for current user)"
}

# Add VCPKG_ROOT
[Environment]::SetEnvironmentVariable("VCPKG_ROOT", "$vcpkgDir", "User")

# Optional: Set CMAKE_TOOLCHAIN_FILE environment variable
[Environment]::SetEnvironmentVariable("CMAKE_TOOLCHAIN_FILE", "$vcpkgDir\scripts\buildsystems\vcpkg.cmake", "User")

Write-Host "vcpkg installed to $vcpkgDir"