#! /usr/bin/bash

# rm ZipCode.out

# g++ --std=c++17 -Wall -o ZipCode.out *.cpp

# ./ZipCode.out post_codes_w_headers.csv

echo "./ZipCode.out -Z56301,501,99950,3 us_postal_codes.lir"

./ZipCode.out -Z56301,501,99950,3 us_postal_codes.lir

exit