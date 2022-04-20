#ifndef LENGTHINDICATEDFILE_H
#define LENGTHINDICATEDFILE_H

#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>

#include "Header.h"
#include "LengthIndicatedBuffer.h"
#include "Place.h"
#include "PrimaryKey.h"
#include "enums.h"

class LengthIndicatedFile {
   private:
    Header header;
    PrimaryKey index;

    LengthIndicatedBuffer readBuf;
    LengthIndicatedBuffer writeBuf;

    std::string fileName;
    std::fstream file;

    int dataStart;

   public:
    LengthIndicatedFile(std::string fileName);
    ~LengthIndicatedFile();
    void initializeBuffers();
    void initializeIndex();
    bool openDataFile();
    std::optional<Place> findRecord(std::string recordKey);
    void generateIndex();
    bool indexFileExists();
};

#endif