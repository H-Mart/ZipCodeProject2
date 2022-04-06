#include <fstream>
#include <iostream>
#include <string> 
#include <iomanip> 

#include "CsvBuffer.h"
#include "extremaTable.h"

/**
 * @brief adding more space between each fields   
 *
 * @param str Used to get the string from csv file 
 * @param c The character value to be spaced  
 * @return string 
 * 
 * @pre string of the fields is searched and found through the argv[2]
 * @post returning the string with a proper format 
 */
std::string addingSpace(std::string str, char c)
{
    std::string s1="";
    for(int i=0;i<str.length();i++) 
    {
        if(str[i]!=c) 
        s1=s1+str[i];
        else 
        s1=s1+"\t"+str[i]+ "\t";
    }
    return s1;
}

/**
 * @brief Reads the csv file passed in as a commandline argument and outputs
 *  a formatted table of the northern, southern, eastern, and westernmost zipcodes in a state.
 *
 * @param argc Used to check if there is an input file
 * @param argv Contains the input file if given
 * @return int
 */
int main(int argc, char const* argv[]) {
    // check to see if there is a command line argument
    if (argc < 2) {
        std::cerr << "No input file given" << std::endl;
        exit(1);
    }

    std::ifstream file(argv[1]);

    if (!file) {
        std::cerr << "Input file cannot be opened. (might not exist)" << std::endl;
        exit(1);
    }

    CsvBuffer buf;

    ExtremaTable table;

    buf.init(file);

    while (!file.eof()) {
        buf.read(file);
        Place p;
        p.unpack(buf);
        table.update(p);
        
        
        // check if there are three arguments assuming user wants to look for the data by zipcode 
        if (argc == 3) {
            for (int i = 0; i < argc; i++) {
                std::string zipSearch = argv[2]; 
                std::string z = "-Z"; 
                std::string line; 

                // find the string "-Z" in the argument argv[2]
                if (zipSearch.find(z) != std::string::npos) 
                {  
                    // remove -Z command character 
                    const std::string newZip = zipSearch.erase(0,2); 

                    // search for zipcode through the file 
                    // expecting to change for the length indicated files! 
                    std::cout << "searching for the zip address of: " << zipSearch << std::endl; 
                    while (std::getline(file, line) && !file.eof())
                    { 
                        // substring to compare newZip 
                        std::string subS = line.substr(0, 5); 

                        int found; 
                        if ((found = subS.find(newZip)) != line.npos) 
                        {
                            // convert string to int 
                            int zip1 = stoi(subS); 
                            int zip2 = stoi(newZip); 

                            // check if substring matches string 
                            if (zip1 == zip2) {
                                std::cout << "zip found!" << std::endl; 
                                          
                                std::cout << std::endl; 
                                std::cout << std::setfill('-') << std::setw(97) << "-" << std::endl;
                            
                               
                                char c = ','; 
                                line = addingSpace(line, c);  
                                 // replace the ',' characters from records 
                                replace(line.begin(), line.end(), ',', ' '); 
                                
                                // labeled fields
                                std::cout << "Zip\t\t"
                                        << "Place Name\t\t"
                                        << "State\t\t"
                                        << "County\t\t"
                                        << "Latitude\t"
                                        << "Longitude\t"
                                        << std::endl;


                                std::cout << std::setfill('-') << std::setw(97) << "-" << std::endl; 
                                std::cout << line << std::endl;  
                                std::cout << std::setfill('-') << std::setw(97) << "-" << std::endl;
                            
                            }
                            else {
                                // newZip does not match with substring subS
                                std::cout << "zip not found!" << std::endl; 
                            }
                            // close file 
                            file.close();
                        }
                    } 
                    exit(0);
                }
                else {
                    // command not found or does not include -Z 
                    std::cerr << "invalid command" << std::endl; 
                    exit(0); 
                }
            }
        }
    }

    // if less than 3 arguments in command line  
    // print table
    std::cout << table;

    return 0;
}
