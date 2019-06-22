#!/bin/bash

set -e
set -x

if [ "$TRAVIS_OS_NAME" = "osx" ];
then
    if which pyenv > /dev/null;
    then
        eval "$(pyenv init -)"
    fi
    pyenv activate conan
fi

python ./Scripts/CI/Conan/build.py
