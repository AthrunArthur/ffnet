#!/bin/bash
path=`pwd`
protoc -I=$path --cpp_out=$path $path/message.proto
echo "protoc -I=$path --cpp_out=$path $path/message.proto"

cd $path/build
cmake ../
make

