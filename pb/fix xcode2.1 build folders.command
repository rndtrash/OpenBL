#!/bin/bash
if [ -e build ]; then
	mv build build-old-`date "+%m.%d.%C%y-%H.%M.%S"`
fi
mkdir build
cd build
ln -s ../../example Default
ln -s ../../example Development
ln -s ../../example Deployment

