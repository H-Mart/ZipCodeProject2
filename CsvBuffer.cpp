#include "CsvBuffer.h"

#include <iostream>
#include <regex>

CsvBuffer::CsvBuffer(const char delim) : delim(delim){};

void CsvBuffer::init(std::istream& instream) {
    read(instream);
    readHeader();
}

void CsvBuffer::read(std::istream& instream) {
    char c;
    bool inQuotes = false;

    curr = 0;
    buffer.clear();

    while (!instream.eof()) {
        instream.get(c);

        if (c == '\n' && !inQuotes) {
            buffer.push_back(c);
            break;
        } else if (c == '"') {
            inQuotes = !inQuotes;
        }
        
        buffer.push_back(c);
    }
}

bool CsvBuffer::unpack(std::string& str) {
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
                } else if (c == '\n') {
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
        curr++;
    }
    return recordHasMore;
}

std::pair<HeaderField, std::string> CsvBuffer::getCurFieldHeader() {
    return headers[fieldNum];
}

HeaderField getFieldType(std::string headerValue) {
    std::regex zipCodePat("Zip\\s*Code");
    std::regex placeNamePat("Place\\s*Name");
    std::regex statePat("State");
    std::regex countyPat("County");
    std::regex latitudePat("Lat");
    std::regex longitudePat("Long");

    if (std::regex_search(headerValue, zipCodePat)) {
        return HeaderField::ZipCode;
    } else if (std::regex_search(headerValue, placeNamePat)) {
        return HeaderField::PlaceName;
    } else if (std::regex_search(headerValue, statePat)) {
        return HeaderField::State;
    } else if (std::regex_search(headerValue, countyPat)) {
        return HeaderField::County;
    } else if (std::regex_search(headerValue, latitudePat)) {
        return HeaderField::Latitude;
    } else if (std::regex_search(headerValue, longitudePat)) {
        return HeaderField::Longitude;
    } else {
        return HeaderField::Unknown;
    }
}

void CsvBuffer::readHeader() {
    bool more = true;
    while (more) {
        std::string temp;
        more = unpack(temp);
        headers.push_back({getFieldType(temp), temp});
    }
    numFields = headers.size();
}
