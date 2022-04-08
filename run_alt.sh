#! /usr/bin/bash

rm ZipCode.out

g++ --std=c++17 -Wall -o ZipCode.out *.cpp

./ZipCode.out ./csvs/sorted_county.csv  sorted_county.out
./ZipCode.out ./csvs/sorted_lat.csv  sorted_lat.out
./ZipCode.out ./csvs/sorted_long.csv  sorted_long.out
./ZipCode.out ./csvs/sorted_pl_name.csv  sorted_pl_name.out
./ZipCode.out ./csvs/sorted_state.csv  sorted_state.out
./ZipCode.out ./csvs/sorted_zip.csv  sorted_zip.out


stat sorted_county.out -c "%s"
stat sorted_lat.out -c "%s"
stat sorted_long.out -c "%s"
stat sorted_pl_name.out -c "%s"
stat sorted_state.out -c "%s"
stat sorted_zip.out -c "%s"


# diff sorted_zip.out sorted_county.out 
# diff sorted_zip.out sorted_lat.out
# diff sorted_zip.out sorted_long.out 
# diff sorted_zip.out sorted_pl_name.out 
# diff sorted_zip.out sorted_state.out 