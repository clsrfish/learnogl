#!/bin/sh
cdir=$(pwd)

pdir=$(dirname "${0}")
cd ${pdir}/build
rm -rfv $(ls -A | grep -v '.gitkeep')

cd ${cdir}
