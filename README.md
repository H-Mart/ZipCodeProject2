# ZipCodeProject2

Note that lirf refers to the data file type we've designed for this project. It stands for Length Indicated Record File/Format. 

## Compiling
Compile with `g++ --std=c++17 -Wall -o ZipCode.out *.cpp -lstdc++fs` 

(-lstdc++fs links the filesystem library and must be at the end of the call to g++)

## Running
There are two optional arguments that will change the way the program functions.

  `-Z<list_of_zip_codes>` where `list_of_zip_codes` is a comma separated list of zip codes to search the lirf file for
  
  `-C<csv_filename>` where `csv_filename` is the name of a csv file of the type used in this project.

The only required argument is a path to a lirf file. 

If `-C` is used, the program will try to convert the csv file to the lirf format and write it to the lirf path provided.  
If `-Z` is used, the program will load the lirf file from the path provided and search for the zip codes.  
If both `-C` and `-Z` are used, the program will currently act as if only `-C` was used. (This might change later)

These options can appear in any order after the executable name. 

For example: 

- `./ZipCode.out -Z24321,42421 example_lirf_file`  
will try to open and read a file called `example_lirf_file` and search for the zip codes `24321` and `42421`.

- `./ZipCode.out -Cexample_csv_file.csv example_lirf_file`  
will try to open and read a file called `example_csv_file.csv` and will create or overwrite a lirf file called `example_lirf_file` with the data read from the csv file. 

- `./ZipCode.out example_lirf_file`  
without either optional argument, the program will simply try to read `example_lirf_file` as a lirf file and do nothing. 
