#include "PrimaryKey.h"
#include <sstream>
#include <algorithm>

bool CompareStr(PrimaryKey::KeyStruct& s1, PrimaryKey::KeyStruct& s2){
    return s1.key.length() < s2.key.length() || (s1.key.length() == s2.key.length() && s1.key < s2.key);
}

void PrimaryKey::GenerateIndexFile(std::string fileName) {
    std::ofstream ofile(fileName, std::ios::binary);

    std::sort(vKey.begin(), vKey.end(), CompareStr);		//Sort the list by string length and value
	
    for (size_t i = 0; i < vKey.size(); i++) {
        ofile << vKey[i].key << "," << vKey[i].offset << '\n'; 	//Replaced space with comma (delimiter for strings with spaces)
    }

    ofile.close();
}

bool PrimaryKey::ReadIndexFile(std::string fileName) {
    std::ifstream iFile(fileName, std::ios::binary);

    if (!iFile.is_open())
        return false;

    vKey.clear();

    std::string line, str;
    unsigned int offset;
	
    while (std::getline(iFile, line)) {				//Reworked so that strings with spaces can be used as primary keys
        std::stringstream ss(line);
	std::getline(ss, str, ',');
	ss >> offset;
	vKey.push_back({ str, offset });
    }

    iFile.close();
    return true;
}

void PrimaryKey::Add(KeyStruct keyStruct) {
    vKey.push_back(keyStruct);
}

int PrimaryKey::BinarySearch(std::string key) {			//Binary search for string type
	int left = 0,
	int right = vKey.size()-1,
	int middle;

	while (left <= right) {
		middle = (left + right - 1) / 2;

		std::string& k = vKey[middle].key;
		if (key == k)
			return vKey[middle].offset;
		
		if (key.length() > k.length() || (key > k && key.length() == k.length()))
			left = middle + 1;
		else
			right = middle - 1;
	}

	return -1;
}

int PrimaryKey::Find(std::string key) {
    for (auto& e : vKey)
        if (key == e.key)
            return e.offset;

    return -1;
}
