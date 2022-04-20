#pragma once
#include <fstream>

#include "Header.h"

class PrimaryKey {

   public:
    static const int notFound = -1;
    enum IndexFileFormat {
		ASCII,
		BCD,
		BINARY
	};
	 struct IndexFileHeader {
		int version;
		int keyCount;
		int format;
	};
    struct KeyStruct {
        std::string key;
        unsigned int offset;
    };
    /**
     * @brief Generates an index file from the internal index
     * 
     * @param fileName the name of the index file that will be created
     */
    void GenerateIndexFile(std::string fileName);

    /**
     * @brief Reads an index file and stores the values in the internal index. 
     * Also records if the index's primary keys are in ascending order, to facilitate binary search
     * 
     * @param fileName the name of the index file to be read
     * @return true file was read successfully
     * @return false file was unable to be read successfully
     */
    bool ReadIndexFile(std::string fileName);

    /**
     * @brief Adds a key, value pair to the index
     * 
     * @param keyStruct the struct containing the key, value pair to be added
     */
    void Add(KeyStruct keyStruct);

    /**
     * @brief Performs a linear search on the internal index to try to find the given key
     * 
     * @param key the zip code to search for
     * @return int if >= 0, the return value is the offset from the start of the file to the start of the record
     * @return int if = -1, the key could not be found in the record
     */
    int Find(std::string key);

    /**
     * @brief Performs a binary search on the internal index to try to find the given key
     * 
     * @pre list does not have to be sorted since the first thing the function checks is if it is, then sorts it if it is not
     * @post internal list is sorted in ascending order
     * 
     * @param key the zip code to search for
     * @return int if >= 0, the return value is the offset from the start of the file to the start of the record
     * @return int if = -1, the key could not be found in the record
     */
    int BinarySearch(std::string str);

   private:
    std::vector<KeyStruct> vKey;
    bool isSorted = false;
    IndexFileHeader header;
};