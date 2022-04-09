#ifndef HEADER_BUFFER_H
#define HEADER_BUFFER_H

#include <iostream>
#include <vector>

#include "Header.h"

class HeaderBuffer {
   private:
    std::vector<unsigned char> buffer;

   public:
    /**
     * @brief Read header file into buffer
     *
     * @param[in] ins stream to be read in from
     *
     * @pre ins is pointing to an open length indicated file and ins.good() is true
     * @post ins is positioned at the first character after the header
     * @post buffer is filled with header bytes
     *
     */
    void read(std::istream& ins);

    /**
     * @brief Unpacks header fields from buffer into Header object
     *
     *
     * @pre read() has been called on a valid file and buffer contains a header with the Header format
     *
     * @return a Header object loaded with the information in the file
     */
    Header unpack();
};

#endif  // HEADER_BUFFER_H