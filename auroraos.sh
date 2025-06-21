#!/bin/bash

if [[ "$1" == "clean" ]]; then
  python3 meta/build.py clean
fi
set -e

function is_ubuntu() {
    if [ -f /etc/os-release ]; then
        grep -q "ubuntu" /etc/os-release
        return $?
    fi
    return 1
}

function is_darwin() {
    if [ "$(uname)" == "Darwin" ]; then
        return 0
    fi
    return 1
}

  
if is_ubuntu; then
    echo "Detected Ubuntu, installing dependencies automatically..."
    sudo apt-get install gcc g++ qemu-system-i386 python3 -y
elif is_darwin; then
    echo "Detected macOS, installing dependencies automatically..."
    if ! command -v brew &> /dev/null
    then
        echo "Homebrew is not installed. Please install Homebrew first."
        echo '/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"'
        exit 1
    fi
          
    brew install gcc qemu python3
else
  echo "Unsupported OS. Please install dependencies manually."
  exit 1
fi

python3 meta/build.py
