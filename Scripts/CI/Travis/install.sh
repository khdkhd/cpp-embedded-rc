#!/bin/bash

set -e
set -x

# if [[ "$(uname -s)" == 'Darwin' ]]; then
#     MACOS_HEADER_PKG="/Library/Developer/CommandLineTools/Packages/macOS_SDK_headers_for_macOS_10.14.pkg"
#
#     if [ -f "$MACOS_HEADER_PKG" ]; then
#         sudo installer -pkg "$MACOS_HEADER_PKG" -target /
#     fi
#
#     brew update
#     brew outdated pyenv || brew upgrade pyenv
#     brew install pyenv-virtualenv
#     brew install cmake || true
#
#     if which pyenv > /dev/null; then
#         eval "$(pyenv init -)"
#     fi
#
#     pyenv install 3.7.3
#     pyenv virtualenv 3.7.3 conan
#     pyenv rehash
#     pyenv activate conan
# fi

pip install conan --upgrade
pip install conan_package_tools

conan user
