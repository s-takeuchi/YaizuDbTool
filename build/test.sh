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
cp cmdfreak.conf $BUILDDIR
cp cmdfreak.dat $BUILDDIR
echo servicehost=localhost >> $BUILDDIR/cmdfreak.conf
echo serviceport=10008 >> $BUILDDIR/cmdfreak.conf
cd ../../test/restapitest
make all
cp restapitest $BUILDDIR


# Testing
cd $BUILDDIR
echo
echo Test starts
./cmdfreak&
./restapitest
echo Test ends
