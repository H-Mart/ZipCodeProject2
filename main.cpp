#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include "CsvBuffer.h"
#include "HeaderBuffer.h"
#include "LengthIndicatedBuffer.h"
#include "Place.h"

int transferRecords(std::istream& csvFile, std::ostream& liFile, CsvBuffer& buf, LengthIndicatedBuffer& lBuf) {
    int recCount = 0;

    while (!csvFile.eof()) {
        buf.read(csvFile);
        Place p;
        p.unpack(buf);

        p.pack(lBuf);
        lBuf.write(liFile);
        recCount++;
    }

    lBuf.header.fileInfo.recordCount = recCount;

    lBuf.writeHeader(liFile);
}

void convertFileType(std::istream& csvFile, std::ostream& liFile, std::string csvFileName) {
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

        field.fieldType = (int)type;
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
            (int)LengthIndicatorType::ASCII,  // length indicator type
            0,                                // number of records (will be set later)
            6,                                // number of fields (will be set later)
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

    std::cout << headerSize;
    header.fileInfo.fieldsPerRecord = numFields;
    header.headerInfo.headerSize = headerSize;
    indexFileName.copy(header.fileInfo.indexFileName, 100);

    liFile << header;
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

    // LengthIndicatedBuffer works the same way as the CsvBuffer,
    // before doing anything, pass an open ifstream pointing to the length indicated file to the init method,
    // then passing the same ifstream to the read method will read one record into the buffer
    // passing a place object to the unpack method will then unpack the buffer into the place object
    // read returns true if there are more records in the file, false if there are no more records

    std::ifstream file(argv[1], std::ios::binary);

    if (!file) {
        std::cerr << "Input file cannot be opened. (might not exist)" << std::endl;
        exit(1);
    }

    // CsvBuffer buf;
    // LengthIndicatedBuffer lBuf;

    // convertFileType(file, file3, argv[1]);

    // std::ifstream file2(argv[2], std::ios::binary);

    // buf.init(file);
    // lBuf.init(file2);
    // file2.close();

    // std::ofstream file3(argv[2], std::ios::binary);
    // transferRecords(file, file3, buf, lBuf);
    // file.close();
    // file3.close();
    return 0;
}
