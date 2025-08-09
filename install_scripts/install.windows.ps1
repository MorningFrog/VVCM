# Install current package (run this script in the project root directory)

if (-Not (Test-Path "pyproject.toml")) {
  Write-Host "Please run this script in the root directory of the project."
  exit 1
}

python -m pip install --no-build-isolation -v .