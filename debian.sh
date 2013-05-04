#!/bin/bash

APP=fin
VERSION=1.51
ARCH_TYPE=`uname -m`

if [ $ARCH_TYPE == "x86_64" ]; then
    ARCH_TYPE="amd64"
fi
if [ $ARCH_TYPE == "i686" ]; then
    ARCH_TYPE="i386"
fi

# Create a source archive
make clean
make source

# Build the package
fakeroot dpkg-buildpackage -F

# sign files
gpg -ba ../${APP}_${VERSION}-1_${ARCH_TYPE}.deb
gpg -ba ../${APP}_${VERSION}.orig.tar.gz
