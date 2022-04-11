#pragma once
#include <fstream>

#include "Header.h"

class PrimaryKey {

   public:
    static const int notFound = -1;
    struct KeyStruct {
        std::string key;
        unsigned int offset;
    };
    void GenerateIndexFile(std::string fileName);
    bool ReadIndexFile(std::string fileName);
    void Add(KeyStruct keyStruct);
    int Find(std::string key);
    int BinarySearch(std::string str);


   private:
    std::vector<KeyStruct> vKey;
    bool isSorted = false;
};