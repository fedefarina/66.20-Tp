#!/usr/bin/env bash

root_dir=`pwd`
install_dir="$root_dir/temp_install"

#Create install dir
mkdir -p temp_install && \
cd $install_dir && \

#Install cmocka
wget https://github.com/clibs/cmocka/archive/master.zip
unzip master.zip && \
rm master.zip && \
cd cmocka-master && rm -r example/* && \
mkdir build && cd build && \
cmake -DCMAKE_BUILD_TYPE=Debug .. && \
make && make install

#Remove install dir
cd $root_dir
rm -rf $install_dir