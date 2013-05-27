#!/bin/bash

APP=fin
PREV_VERSION=1.51
VERSION=1.51
ARCH_TYPE=`uname -m`
DIR=${APP}-${VERSION}

if [ $ARCH_TYPE == "x86_64" ]; then
    ARCH_TYPE="amd64"
fi
if [ $ARCH_TYPE == "i686" ]; then
    ARCH_TYPE="i386"
fi

#update version numbers automatically - so you don't have to
sed -i 's/VERSION='${PREV_VERSION}'/VERSION='${VERSION}'/g' Makefile fedora.sh
sed -i 's/Version: '${PREV_VERSION}'/Version: '${VERSION}'/g' rpmpackage/${APP}.spec
sed -i 's/"'${PREV_VERSION}'"/"'${VERSION}'"/g' src/globals.h 

# change the parent directory name to debian format
mv ../${APP} ../${DIR}

# Create a source archive
make clean
make source

# Build the package
dpkg-buildpackage -F

# sign files
gpg -ba ../${APP}_${VERSION}-1_${ARCH_TYPE}.deb
gpg -ba ../${APP}_${VERSION}.orig.tar.gz

# restore the parent directory name
mv ../${DIR} ../${APP}
