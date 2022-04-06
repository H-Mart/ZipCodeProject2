#ifndef HEADER_BUFFER_H
#define HEADER_BUFFER_H

#include <vector>
#include <iostream>
#include "Header.h"

class HeaderBuffer {
   private:
    std::vector<unsigned char> buffer;
   public:
    void read(std::istream& ins);
    Header unpack();
};

#endif  // HEADER_BUFFER_H