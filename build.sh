#!/bin/bash

#如果文件夹不存在，创建文件夹
if [ ! -d "$PWD/build" ]; then
  mkdir $PWD/build
fi

cmake -B$PWD/build 
cmake --build $PWD/build