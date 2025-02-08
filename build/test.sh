#!/bin/bash -x

# Make build folder
BUILDDIR=$(pwd)/server
if [ -e $BUILDDIR ]; then
    yes|rm -r $BUILDDIR
fi
mkdir -p $BUILDDIR


# Build
cd ../src/restapi
make all
cp serval $BUILDDIR
cp sample.conf $BUILDDIR
cp sample.dat $BUILDDIR
echo servicehost=localhost >> $BUILDDIR/serval.conf
echo serviceport=10009 >> $BUILDDIR/serval.conf
cd ../../test/restapitest
make all
cp restapitest $BUILDDIR


# Testing
cd $BUILDDIR
echo
echo Test starts
./sample&
./restapitest
echo Test ends
