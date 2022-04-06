#include <fstream>
#include <iostream>
#include <string> 
#include <iomanip> 

#include "CsvBuffer.h"
#include "HeaderBuffer.h"
#include "LengthIndicatedBuffer.h"
#include "Place.h"

Header writeHeader(std::ostream& outstream) {
    FieldInfo f1 = {"Zip\nCode", (int)HeaderField::ZipCode};
    FieldInfo f2 = {"Place\nName", (int)HeaderField::PlaceName};
    FieldInfo f3 = {"State", (int)HeaderField::State};
    FieldInfo f4 = {"County", (int)HeaderField::County};
    FieldInfo f5 = {"Lat", (int)HeaderField::Latitude};
    FieldInfo f6 = {"Long", (int)HeaderField::Longitude};
    std::vector<FieldInfo> fields = {f1, f2, f3, f4, f5, f6};

    Header header = {
        {{'Z', 'C', '0', '2'}, 1, 244},
        {2, (int)LengthIndicatorType::ASCII, 1, 2, 1, "IndexFileNameTemp"},
        {fields}};

    header.headerInfo.headerSize = sizeof(header.headerInfo) + sizeof(header.fileInfo);

    for (auto f : fields) {
        header.headerInfo.headerSize += sizeof(f);
    }

    outstream << header;
    return header;
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

    std::ifstream file(argv[1], std::ios::binary);
    std::ofstream file2(argv[2], std::ios::binary);

    if (!file) {
        std::cerr << "Input file cannot be opened. (might not exist)" << std::endl;
        exit(1);
    }

    CsvBuffer buf;
    LengthIndicatedBuffer lBuf;

    buf.init(file);
    lBuf.header = writeHeader(file2);

    int recCount = 0;

    while (!file.eof()) {
        buf.read(file);
        Place p;
        p.unpack(buf);
        p.pack(lBuf);
        lBuf.write(file2);
        recCount++;
    }
    lBuf.header.fileInfo.recordCount = recCount;

    
    file.close();
    file2.close();

    std::ifstream file3(argv[2], std::ios::binary);

    lBuf.init(file3);

    while (!file3.eof()) {
        lBuf.read(file);
        Place p;
        p.unpack(lBuf);

        std::cout << "Zip Code: " << p.getZipCode()
                  << "\nPlace Name: " << p.getName()
                  << "\nState: " << p.getState()
                  << "\nCounty: " << p.getCounty()
                  << "\nLat: " << p.getLat()
                  << "\nLong: " << p.getLongi() << std::endl;
    }

    return 0;
}
