#!/usr/bin/env bash
# Copy all CMakeLists.txt and *.cmake from current folder to dest folder and 
# create the relative path name.

dest=~/test/Csim_PJ4/trunk

files=`find . -name CMakeLists.txt`
files="$files `find . -name \*.cmake`"

for file in $files
do
  echo $file
  dir="$(dirname $file)"
  echo $dir
  mkdir -p $dest/$dir
  cp $file $dest/$dir/.
done

