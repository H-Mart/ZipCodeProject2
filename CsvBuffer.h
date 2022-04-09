#ifndef CSVBUFFER_H
#define CSVBUFFER_H

#include <istream>
#include <string>
#include <vector>

#include "enums.h"

class CsvBuffer {
   private:
    const char delim;

    std::string buffer;

    /// holds the position of the start of unprocessed fields
    size_t curr;

    /// number of records currently in the unprocessed part of the buffer
    // size_t recordCount;
    /// keeps track of how many fields of a record have been processed
    size_t fieldNum;
    /// number of fields in each record
    size_t numFields;

    // first part holds the header type for use when unpacking,
    // second part holds the actual header value
    std::vector<std::pair<HeaderField, std::string>> headers;

    /**
     * @brief Reads through the first record in the file and extracts the fields for use as metadata.
     *
     * @pre headers is an empty vector and the buffer has had data read into it
     *
     * @post headers contains the pairs of header field types and the values of the header fields\n
     *       curr points to the first record\n
     *       numFields is equal to number of header fields found
     */
    void readHeader();

   public:
    /**
     * @brief Construct a new Csv Buffer object
     *
     * @param delim The delimiter used in the csv file
     *
     */
    CsvBuffer(const char delim = ',');

    /**
     * @brief Reads one record into the buffer if there is data left in the stream.
     *
     * @param[in] instream one record will be read
     *
     * @pre instream is an open stream that contains data in a CSV format
     * @post buffer contains data to be unpacked \n instream points to next record or end of stream
     *
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
     *       curr is pointing to the start of the next field or the end of the record
     *
     */
    bool unpack(std::string& str);

    /**
     * @brief Performs the first read and extracts the headers.
     *
     * @param[in] instream stream to be read from
     *
     * @pre buffer is empty
     * @post headers contains the values returned by readHeader\n
     *       buffer contains one unprocessed record\n
     *       curr points to the start of the record.\n
     *       fieldNum is increased by one if the record contains more fields or is set to zero if the entire record has been read.
     */
    void init(std::istream& instream);

    /**
     * @brief Gets the type and value of the current field.
     *
     * @pre headers has been initialized
     *
     * @post returns a pair containing the HeaderField type and the string value of the current field's header
     *
     * @return std::pair<HeaderField, std::string>
     */
    std::pair<HeaderField, std::string> getCurFieldHeader();

    std::vector<std::pair<HeaderField, std::string>> getHeaders() const;
};

#endif  // CSVBUFFER_H
