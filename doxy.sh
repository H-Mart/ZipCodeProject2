#! /usr/bin/bash

doxygen ./Doxyfile *.cpp

pushd docs/latex

make

cp refman.pdf ../../

popd

rm -r docs/

