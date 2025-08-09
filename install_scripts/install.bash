#!/bin/bash

# Install current package in Linux

# Check if in project root directory
if [ ! -f "pyproject.toml" ]; then
  echo "Please run this script in the project root directory"
  exit 1
fi

pip install --no-build-isolation -v .