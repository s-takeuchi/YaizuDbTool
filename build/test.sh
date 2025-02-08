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
cp cmdfreak $BUILDDIR
cp sample.conf $BUILDDIR
cp sample.dat $BUILDDIR
echo servicehost=localhost >> $BUILDDIR/sample.conf
echo serviceport=10008 >> $BUILDDIR/sample.conf
cd ../../test/restapitest
make all
cp restapitest $BUILDDIR


# Testing
cd $BUILDDIR
echo
echo Test starts
./cmdfreak&
sleep 3
./restapitest
echo Test ends
