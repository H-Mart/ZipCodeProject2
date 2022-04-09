#pragma once
#include <fstream>
#include <unordered_set>

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
    int BinarySearch(std::string str);
    int Find(std::string key);

   private:
    std::vector<KeyStruct> vKey;
};
