
#include "LengthIndicatedBuffer.h"

#include <iomanip>
#include <iostream>
#include <regex>
#include <sstream>
#include <vector>

#include "HeaderBuffer.h"

// const char MAGIC_HEADER_NUMBER[4] = {'Z', 'C', '0', '2'};

LengthIndicatedBuffer::LengthIndicatedBuffer(const size_t size, const char delim) : maxSize(size), delim(delim) {
    clear();
}

void LengthIndicatedBuffer::init(std::istream& instream) {
    HeaderBuffer hBuf;

    hBuf.read(instream);
    header = hBuf.unpack();

    if (header.fileInfo.recordCount > 0) {
        std::cout << header.fileInfo.recordCount << std::endl;
        read(instream);
    }
}

void LengthIndicatedBuffer::read(std::istream& instream) {
    // get length of record
    auto lengthIndicatorLength = header.fileInfo.lengthIndicatorSize;

    int recordLength = 0;

    switch (LengthIndicatorType(header.fileInfo.lengthIndicatorFormat)) {
        case LengthIndicatorType::BINARY:
            instream.read((char*)&recordLength, lengthIndicatorLength);
            break;
        case LengthIndicatorType::ASCII:
            std::string recordLengthStr;
            for (int i = 0; i < lengthIndicatorLength; i++) {
                recordLengthStr.push_back(instream.get());
            }
            std::cout << recordLengthStr << std::endl;
            recordLength = std::stoi(recordLengthStr);
    }
    instream.read(buffer, recordLength);
    this->recordLength = recordLength;
}

void LengthIndicatedBuffer::writeHeader(std::ostream& outstream) {
    
}

// void LengthIndicatedBuffer::readHeader(std::istream& instream) {
//     instream >> header.headerInfo;
//     instream >> header.fileInfo;

//     for (int i = 0; i < header.fileInfo.fieldsPerRecord; i++) {
//         FieldInfo fInfo;
//         instream >> fInfo;
//         header.fields.push_back(fInfo);
//     }
// }

bool LengthIndicatedBuffer::unpack(std::string& str) {
    auto state = CSVState::UnquotedField;  // assume field is not quoted by default

    bool fieldHasMore = true;
    bool recordHasMore = true;
    while (fieldHasMore) {
        char c = buffer[curr];
        switch (state) {
            case CSVState::UnquotedField:
                if (c == delim) {
                    fieldHasMore = false;
                    fieldNum++;
                } else if (curr == recordLength) {
                    fieldHasMore = false;
                    recordHasMore = false;
                    fieldNum = 0;
                } else if (c == '"') {
                    state = CSVState::QuotedField;
                } else {
                    str.push_back(c);
                }
                break;
            case CSVState::QuotedField:
                if (c == '"') {
                    state = CSVState::QuotedQuote;
                } else {
                    str.push_back(c);
                }
                break;
            case CSVState::QuotedQuote:
                if (c == delim) {
                    fieldHasMore = false;
                    fieldNum++;
                } else if (c == '"') {
                    str.push_back(c);
                    state = CSVState::QuotedField;
                } else {
                    state = CSVState::UnquotedField;
                }
                break;
        }
        curr = (curr + 1) % maxSize;
    }
    return recordHasMore;
}

void LengthIndicatedBuffer::clear() {
    curr = 0;
}

void LengthIndicatedBuffer::pack(const std::string str) {
    // put delimiters in between each field
    if (curr > 0) {
        buffer[curr++] = delim;
    }

    // note that the size method of std::string does not include
    // the null terminator in the length, thus we are just copying the
    // values in the string to the buffer, as desired
    memcpy(&buffer[curr], str.c_str(), str.size());

    // move curr pointer to the end of the field we just added
    curr += str.size();
}

void LengthIndicatedBuffer::write(std::ostream& outstream) {
    switch (LengthIndicatorType(header.fileInfo.lengthIndicatorFormat)) {
        case LengthIndicatorType::ASCII:
            std::ostringstream lengthStream;
            lengthStream << std::setfill('0') << std::setw(header.fileInfo.lengthIndicatorSize) << curr;
            auto lengthStr = lengthStream.str();
            outstream.write(lengthStr.c_str(), lengthStream.str().size());
            break;
    }

    outstream.write(buffer, curr);
}

std::pair<HeaderField, std::string> LengthIndicatedBuffer::getCurFieldHeader() {
    return headers[fieldNum];
}

// HeaderField getFieldType(std::string headerValue) {
//     std::regex zipCodePat("Zip\\s*Code");
//     std::regex placeNamePat("Place\\s*Name");
//     std::regex statePat("State");
//     std::regex countyPat("County");
//     std::regex latitudePat("Lat");
//     std::regex longitudePat("Long");

//     if (std::regex_search(headerValue, zipCodePat)) {
//         return HeaderField::ZipCode;
//     } else if (std::regex_search(headerValue, placeNamePat)) {
//         return HeaderField::PlaceName;
//     } else if (std::regex_search(headerValue, statePat)) {
//         return HeaderField::State;
//     } else if (std::regex_search(headerValue, countyPat)) {
//         return HeaderField::County;
//     } else if (std::regex_search(headerValue, latitudePat)) {
//         return HeaderField::Latitude;
//     } else if (std::regex_search(headerValue, longitudePat)) {
//         return HeaderField::Longitude;
//     } else {
//         return HeaderField::Unknown;
//     }
// }