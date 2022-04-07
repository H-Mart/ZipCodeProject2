#ifndef LIBUFFER_H
#define LIBUFFER_H

#include <istream>
#include <string>
#include <vector>

#include "Header.h"
#include "helpers.h"

class LengthIndicatedBuffer {
   public:
    const size_t maxSize;
    const char delim;
    int recordLength;

    Header header;

    char buffer[1000];

    /// holds the position of the start of unprocessed fields
    size_t curr;

    /// keeps track of how many fields of a record have been processed
    size_t fieldNum = 0;
    /// number of fields in each record
    size_t numFields;

    // first part holds the header type for use when unpacking,
    // second part holds the actual header value
    std::vector<std::pair<HeaderField, std::string>> headers;

   public:
    /**
     * @brief Construct a new Csv Buffer object
     *
     * @param size The max size of the buffer
     * @param delim The delimiter used in the csv file
     *
     * @pre size > 0
     */
    LengthIndicatedBuffer(const size_t size = 4096, const char delim = ',');

    /**
     * @brief Reads into the buffer getAvailSpace amount of data or to the end of the stream, whichever is smaller.
     *
     * @param[in] instream amount returned by getAvailSpace will be read from instream
     *
     * @pre instream is an open stream that contains data in a CSV format
     * @post buffer contains data to be unpacked from curr to head\n
     *       sets recordCount equal to number of records found while reading
     *
     */
    bool read(std::istream& instream);

    /**
     * @brief Reads a field and puts it into a string
     *
     * @param[out] str the string that will hold the value of the field
     * @return true record has not had every field unpacked
     * @return false record has no more fields to unpack
     *
     * @pre curr is pointing to the start of a field
     *      str is an empty std::string
     *
     * @post str contains the value of the field
     *       curr is pointing to the start of the next field or the next record
     *
     */
    bool unpack(std::string& str);

    void pack(const std::string str);
    void write(std::ostream& outstream);

    /**
     * @brief Performs the first read and extracts the headers.
     *
     * @param[in] instream stream to be read from
     *
     * @pre buffer is empty
     * @post headers contains the values returned by readHeader\n
     *       buffer contains raw data\n
     *       curr points to the start of the buffer.\n
     *       head points to the end of the buffer or the amount of data read from the stream, whichever is smaller.\n
     *       fieldNum is increased by one if the record contains more fields or is set to zero if the entire record has been read.
     */
    void init(std::istream& instream);

    void writeHeader(std::ostream& outstream);

    void readHeader(std::istream& instream);

    void clear();

    /**
     * @brief Gets the type and value of the current field.
     *
     * @pre headers has been initialized
     *
     * @post returns a pair containing the HeaderField type and the string value of the current field's header
     *
     * @return std::pair<HeaderField, std::string>
     */
    FieldInfo getCurFieldHeader();
};

#endif  // CSVBUFFER_H
