#include "Place.h"

#include <fstream>
#include <sstream>

using std::string;

Place::Place() {
    zipcode = "";
    state = "";
    name = "";
    latitude = 0;
    county = "";
    longitude = 0;
};

// copy constructor
Place::Place(const Place& loc) {
    zipcode = loc.getZipCode();
    name = loc.getName();
    state = loc.getState();
    county = loc.getCounty();
    latitude = loc.getLat();
    longitude = loc.getLongi();
};

// overload the assignment operator
void Place ::operator=(const Place& loc) {
    this->zipcode = loc.getZipCode();
    this->name = loc.getName();
    this->state = loc.getState();
    this->county = loc.getCounty();
    this->latitude = loc.getLat();
    this->longitude = loc.getLongi();
}

string Place::getZipCode() const { return zipcode; }  // zipcode value
string Place::getName() const { return name; }        // name value
string Place::getState() const { return state; }      // State value
string Place::getCounty() const { return county; }    // county value
double Place::getLat() const { return latitude; }     // Latitude value
double Place::getLongi() const { return longitude; }  // longitude value

// passing to place object by unpacking from buffer
void Place::unpack(CsvBuffer& buffer) {
    std::string skip;
    string lat_str, long_str;

    bool moreFields = true;
    while (moreFields) {
        auto curField = buffer.getCurFieldHeader();
        switch (HeaderField(curField.first)) {
            case HeaderField::ZipCode:
                moreFields = buffer.unpack(zipcode);
                break;
            case HeaderField::PlaceName:
                moreFields = buffer.unpack(name);
                break;
            case HeaderField::State:
                moreFields = buffer.unpack(state);
                break;
            case HeaderField::County:
                moreFields = buffer.unpack(county);
                break;
            case HeaderField::Latitude:
                moreFields = buffer.unpack(lat_str);
                break;
            case HeaderField::Longitude:
                moreFields = buffer.unpack(long_str);
                break;
            default:
                moreFields = buffer.unpack(skip);
                break;
        }
    }

    std::stringstream(lat_str) >> latitude;    // convert to float
    std::stringstream(long_str) >> longitude;  // convert to float
}

void Place::unpack(LengthIndicatedBuffer& buffer) {}

void Place::pack(LengthIndicatedBuffer& buffer) {
    buffer.clear();
    std::stringstream lat_strStream;
    std::stringstream long_strStream;

    lat_strStream << latitude;
    long_strStream << longitude;

    for (auto f : buffer.header.fields) {
        switch (HeaderField(f.fieldType)) {
            case HeaderField::ZipCode:
                buffer.pack(zipcode);
                break;
            case HeaderField::PlaceName:
                buffer.pack(name);
                break;
            case HeaderField::State:
                buffer.pack(state);
                break;
            case HeaderField::County:
                buffer.pack(county);
                break;
            case HeaderField::Latitude:
                buffer.pack(lat_strStream.str());
                break;
            case HeaderField::Longitude:
                buffer.pack(long_strStream.str());
                break;
            default:
                break;
        }
    }
}

size_t Place::getSize() {
    size_t size = 0;
    size += name.size();
    size += zipcode.size();
    size += state.size();
    size += county.size();
    size += sizeof(latitude);
    size += sizeof(longitude);
    return size;
}