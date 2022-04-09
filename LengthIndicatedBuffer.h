#ifndef LIBUFFER_H
#define LIBUFFER_H

#include <array>
#include <istream>
#include <string>
#include <vector>

#include "Header.h"
#include "enums.h"

class LengthIndicatedBuffer {
   private:
    const char delim;
    int recordLength;
    bool initialized = false;


    char buffer[1000];

    /// holds the position of the start of unprocessed fields
    int curr;

    /// keeps track of how many fields of a record have been processed
    size_t fieldNum = 0;
    /// number of fields in each record
    size_t numFields;


   public:
    Header header;


    /**
     * @brief Construct a new LengthIndicatedBuffer object
     *
     * @param delim The delimiter used between the record fields
     *
     */
    LengthIndicatedBuffer(const char delim = ',');

    /**
     * @brief Read a single length indicated record into the buffer
     *
     * @param[in] instream the stream that points to the record
     *
     * @pre instream is an open stream pointing to the start of a length indicated record
     * @pre record size < sizeof(buffer)
     * @post instream is positioned at the start of the next record or the end of the stream
     * @post curr is set to 0 and buffer is zeroed
     *
     * @retval true when instream.good() is true (indicating that we can read another record)
     * @retval false when instream.good() is false (indicating that we are probably at the end of the file)
     */
    bool read(std::istream& instream);

    /**
     * @brief Seek to the specified offset in the file and read a single length indicated record into the buffer
     *
     * @param[in] instream the stream that points to the record
     * @param[in] indexOffset the number of bytes from the start of the file to seek to before reading
     *
     * @pre instream is an open stream pointing to the start of a length indicated record
     * @pre indexOffset is the number of bytes from the start of the file to the start of a valid length indicated record
     *
     * @post instream is positioned at the start of the next record or the end of the stream
     *
     * @retval true when instream.good() is true (indicating that we can read another record)
     * @retval false when instream.good() is false (indicating that we are probably at the end of the file)
     */
    bool read(std::istream& instream, int indexOffset);

    /**
     * @brief Reads a field and puts it into a string
     *
     * @param[out] str the string that will hold the value of the field
     *
     * @pre curr is pointing to the start of a field
     *      str is an empty std::string
     *
     * @post str contains the value of the field
     * @post curr is pointing to the start of the next field or the next record
     * @post fieldNum is pointing to the type of the next field
     *
     * @retval true record has not had every field unpacked
     * @retval false record has no more fields to unpack
     */
    bool unpack(std::string& str);

    /**
     * @brief Packs a field into the buffer
     *
     * @param[in] str the string that will holds the value of the field
     *
     * @pre str is of the correct type indicated by headers[fieldNum].fieldType
     *
     * @post if the field is not the first, buffer has had a comma and the data from str minus the null terminator added
     * @post if the field is first, buffer has had the data from str minus the null terminator added
     * @post curr points to the first position in buffer after the newly added field
     *
     */
    void pack(const std::string str);

    /**
     * @brief Writes length of the field and the data in the buffer to the stream
     *
     * @param[in] str the string that will holds the value of the field
     *
     * @pre str is of the correct type indicated by headers[fieldNum].fieldType
     *
     * @post 
     *
     */
    void write(std::ostream& outstream);

    /**
     * @brief read and extract the header. 
     *
     * @param[in] instream stream to be read from
     *
     * @pre instream points to a valid length indicated file opened for reading
     * @post header has been loaded with the values from the stream
     * @post initialized has been set to true if header read was successful, false if it was not       
     *       
     * @retval true header read was successful
     * @retval false header read was not successful
     *       
     */
    bool init(std::istream& instream);

    /**
     * @brief Seeks to the start of the stream and writes the header member to the stream
     * 
     * @post outstream is pointing to the first byte after the header
     * 
     * @param[out] outstream the stream to be written to
     */
    void writeHeader(std::ostream& outstream);

    /**
     * @brief Sets curr to start of buffer
     * 
     * @post curr = 0
     * 
     */
    void clear();

    /**
     * @brief read the first 4 bytes of the file and check against the magic number. 
     *
     * @param[in] instream stream to be read from
     *
     * @pre instream is open for reading
     *       
     * @retval true if file has correct magic number
     * @retval false if file does not have correct magic number
     *       
     */
    bool checkFileType(std::istream& instream);

    /**
     * @brief get the name of the index file from the header
     *
     * @pre initialized = true
     *       
     * @return string containing index file name
     *       
     */
    std::string getIndexFileName();

    /**
     * @brief Gets the type and value of the current field.
     *
     * @pre headers has been initialized
     *
     * @post returns a FieldInfo struct with the field name and field type
     *
     * @return FieldInfo
     */
    FieldInfo getCurFieldHeader();
};

#endif  // CSVBUFFER_H
