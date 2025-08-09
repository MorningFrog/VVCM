<#
.SYNOPSIS
    Windows automatic installation script for CMake
.DESCRIPTION
    Checks if CMake is installed, if not, installs it via Chocolatey.
#>

# Check if CMake is installed
if (Get-Command cmake.exe -ErrorAction SilentlyContinue) {
    Write-Host "CMake is installed, current version:"
    cmake --version
    exit 0
}

# Check if Chocolatey (Windows package manager) is installed
if (-not (Get-Command choco -ErrorAction SilentlyContinue)) {
    Write-Host "Chocolatey not found, installing..."
    # Install Chocolatey with administrative privileges
    Set-ExecutionPolicy Bypass -Scope Process -Force
    [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072
    Invoke-Expression ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))
}

# Use Chocolatey to install CMake
Write-Host "Installing CMake..."
choco install cmake -y --no-progress

# Verify installation
if (Get-Command cmake.exe -ErrorAction SilentlyContinue) {
    Write-Host "CMake installation successful, version:"
    cmake --version
}
else {
    Write-Host "CMake installation failed, please check your network or install it manually."
    exit 1
}