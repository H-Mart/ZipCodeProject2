#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include "CsvBuffer.h"
#include "HeaderBuffer.h"
#include "LengthIndicatedBuffer.h"
#include "Place.h"
#include "PrimaryKey.h"

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
std::string addingSpace(std::string str, char c) {
    std::string s1 = "";
    for (size_t i = 0; i < str.length(); i++) {
        if (str[i] != c)
            s1 = s1 + str[i];
        else
            s1 = s1 + "\t" + str[i] + "\t";
    }
    return s1;
}

void transferRecords(std::istream& csvFile, std::iostream& lirfFile) {  //, CsvBuffer& buf, LengthIndicatedBuffer& lBuf) {
    LengthIndicatedBuffer lBuf;
    CsvBuffer cBuf;

    csvFile.seekg(0);
    lirfFile.seekg(0);

    lBuf.init(lirfFile);
    cBuf.init(csvFile);

    auto startPos = lBuf.header.headerInfo.headerSize;

    // seek past the header, should be the start of the first record
    lirfFile.seekp(startPos);

    while (cBuf.read(csvFile)) {
        Place p;
        p.unpack(cBuf);

        p.pack(lBuf);
        lBuf.write(lirfFile);
    }
}

void convertFileType(std::istream& csvFile, std::ostream& lirfFile, std::string csvFileName) {
    CsvBuffer csvBuf;
    csvBuf.init(csvFile);

    auto csvHeaders = csvBuf.getHeaders();

    std::string indexFileName = csvFileName.substr(0, 96) + ".idx";

    std::vector<FieldInfo> fields;

    for (auto h : csvHeaders) {
        auto type = h.first;
        auto name = h.second;

        FieldInfo field;

        // clear fieldName array
        memset(field.fieldName, 0, sizeof(field.fieldName));
        name.copy(field.fieldName, sizeof(field.fieldName));

        field.fieldType = type;
        fields.push_back(field);
    }

    Header header = {
        {
            {'Z', 'C', '0', '2'},  // magic number
            1,                     // version number
            0                      // length of header (will be set later)
        },
        {
            2,                                // length indicator length
            LengthIndicatorType::ASCII,  // length indicator type
            (int)csvHeaders.size(),           // number of fields
            0,                                // primary key position
            ""                                // name of the index file (will be set later)
        },
        {}};

    for (auto f : fields) {
        header.fields.push_back(f);
    }

    // clear entire index file name array
    memset(header.fileInfo.indexFileName, 0, 100);

    auto numFields = fields.size();
    auto headerSize = sizeof(header.headerInfo) + sizeof(header.fileInfo) + sizeof(FieldInfo) * numFields;

    header.fileInfo.fieldsPerRecord = numFields;
    header.headerInfo.headerSize = headerSize;

    indexFileName.copy(header.fileInfo.indexFileName, 100);

    lirfFile << header;
}

void loadIndex(PrimaryKey& pKey, std::string indexFileName) {
    pKey.ReadIndexFile(indexFileName);
}

void generateIndex(PrimaryKey& pKey, std::string indexFileName, std::string dataFileName) {
    std::ifstream file(dataFileName, std::ios::binary);

    LengthIndicatedBuffer lBuf;
    lBuf.init(file);

    std::cout << "lBuf.buffer" << std::endl;
    auto pos = (unsigned int)file.tellg();
    while (lBuf.read(file)) {
        Place place;
        place.unpack(lBuf);
        pKey.Add({place.getZipCode(), pos});
        pos = (unsigned int)file.tellg();
    }
    pKey.GenerateIndexFile(indexFileName);
}

std::vector<std::string> parseZipArg(std::string zipList) {
    std::vector<std::string> zips;
    if (!zipList.size()) {  // no zip codes given
        return zips;
    }

    size_t commaPos;
    int offset = 0;
    while ((commaPos = zipList.find(',', offset)) != std::string::npos) {
        zips.push_back(zipList.substr(offset, commaPos - offset));
        offset = commaPos + 1;
    }
    zips.push_back(zipList.substr(offset));
    return zips;
}

void parseArgs(int argc, char const* argv[], std::vector<std::string>& zipList, std::string& csvFileName, std::string& lirfFileName) {
    const std::string zipFlag = "-Z";
    const std::string csvFlag = "-C";

    for (int i = 1; i < argc; i++) {
        auto arg = std::string(argv[i]);
        if (arg.size() > 2) {  // check for flag at beginning of arg
            auto first2 = arg.substr(0, 2);

            if (first2 == zipFlag) {
                zipList = parseZipArg(arg.substr(2));  // send arg string minus flag characters
            } else if (first2 == csvFlag) {
                csvFileName = arg.substr(2);
            } else {  // if no flag, treat it as a length indicated file name
                lirfFileName = arg;
            }
        }
    }
}

void printFoundZips(std::vector<Place>& places) {
    size_t zip_w = 0;
    size_t name_w = 0;
    size_t state_w = 0;
    size_t county_w = 0;
    size_t lat_w = 12;
    size_t long_w = 12;

    for (auto p : places) {
        if (p.getZipCode().size() > zip_w) {
            zip_w = p.getZipCode().size() + 5;
        }
        if (p.getName().size() > name_w) {
            name_w = p.getName().size() + 5;
        }
        if (p.getState().size() > state_w) {
            state_w = p.getState().size() + 5;
        }
        if (p.getCounty().size() > county_w) {
            county_w = p.getCounty().size() + 6;
        }
    }

    size_t total = zip_w + name_w + state_w + county_w + lat_w + long_w;

    std::cout << std::setfill('-') << std::setw(total) << "-" << std::endl;

    std::cout << std::setfill(' ') << std::setw(zip_w) << std::left
              << "Zip" << std::setw(name_w)
              << "Place Name" << std::setw(state_w)
              << "State" << std::setw(county_w)
              << "County" << std::setw(lat_w)
              << "Latitude" << std::setw(long_w)
              << "Longitude"
              << std::endl;

    std::cout << std::setfill('-') << std::setw(total) << "-" << std::endl;
    for (auto place : places) {
        std::cout << std::setfill(' ') << std::setw(zip_w) << std::left
                  << place.getZipCode() << std::setw(name_w)
                  << place.getName() << std::setw(state_w)
                  << place.getState() << std::setw(county_w)
                  << place.getCounty() << std::setw(lat_w)
                  << place.getLat() << std::setw(long_w)
                  << place.getLongi()
                  << std::endl;
    }
    std::cout << std::setfill('-') << std::setw(total) << "-" << std::endl;
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

    std::vector<std::string> zipList;
    std::string csvFileName;
    std::string lirfFileName;

    parseArgs(argc, argv, zipList, csvFileName, lirfFileName);

    // LengthIndicatedBuffer works the same way as the CsvBuffer,
    // before doing anything, pass an open ifstream pointing to the length indicated file to the init method,
    // then passing the same ifstream to the read method will read one record into the buffer
    // passing a place object to the unpack method will then unpack the buffer into the place object
    // read returns true if there are more records in the file, false if there are no more records

    // if we are given a csv file and a lirf file
    // we will convert the csv file to the lirf format
    if (csvFileName.size() && lirfFileName.size()) {
        std::ifstream csvFile(csvFileName, std::ios::binary | std::ios::in | std::ios::out);
        std::fstream lirfFile(lirfFileName, std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);

        convertFileType(csvFile, lirfFile, csvFileName);
        transferRecords(csvFile, lirfFile);

        csvFile.close();
        lirfFile.close();

        return 0;
    }

    if (zipList.size() && lirfFileName.size()) {
        // load index and find zip files
        std::ifstream lirfFile(lirfFileName, std::ios::binary);
        LengthIndicatedBuffer lBuf;

        if (!lBuf.init(lirfFile)) {
            std::cerr << "length indexed file could not be loaded" << std::endl;
            return 1;
        }

        PrimaryKey pKey;
        auto indexFileName = lBuf.getIndexFileName();

        if (std::filesystem::exists(indexFileName)) {
            loadIndex(pKey, indexFileName);
        } else {
            generateIndex(pKey, indexFileName, lirfFileName);
        }

        int offset;
        std::vector<Place> foundPlaces;
        std::vector<Place> notFoundPlaces;
        for (auto zip : zipList) {
            if ((offset = pKey.Find(zip)) != pKey.notFound) {
                lBuf.read(lirfFile, offset);
                Place p;
                p.unpack(lBuf);
                foundPlaces.push_back(p);
            } else {
                // Place p = {zip, "", "", "", 0, 0};
                // foundPlaces.push_back(p);
            }
        }

        if (foundPlaces.size() > 0) {
            printFoundZips(foundPlaces);
        } else {
            std::cout << "Zip Codes not found :(" << std::endl;
        }

        return 0;
    }

    if (lirfFileName.size()) {
        // check if index exists and generate it if it doesn't
        std::ifstream lirfFile(lirfFileName, std::ios::binary);
        LengthIndicatedBuffer lBuf;

        if (!lBuf.init(lirfFile)) {
            std::cerr << "length indexed file could not be loaded" << std::endl;
            return 1;
        }

        PrimaryKey pKey;
        auto indexFileName = lBuf.getIndexFileName();

        if (std::filesystem::exists(indexFileName)) {
            loadIndex(pKey, indexFileName);
        } else {
            generateIndex(pKey, indexFileName, lirfFileName);
        }
    }

    return 0;
}
