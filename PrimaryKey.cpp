#include "PrimaryKey.h"
#include <sstream>

void PrimaryKey::GenerateIndexFile(std::string fileName) {
    std::ofstream ofile(fileName, std::ios::binary);

    for (size_t i = 0; i < vKey.size(); i++) {
        ofile << vKey[i].key << " " << vKey[i].offset << '\n';
    }

    ofile.close();
}

bool PrimaryKey::ReadIndexFile(std::string fileName) {
    std::ifstream iFile(fileName, std::ios::binary);

    if (!iFile.is_open())
        return false;

    vKey.clear();

    std::string str;
    unsigned int offset;

    std::string line;
    while (std::getline(iFile, line)) {
		std::stringstream ss(line);
        ss >> str;
        ss >> offset;
        vKey.push_back({str, offset});
    }

    iFile.close();
    return true;
}

void PrimaryKey::Add(KeyStruct keyStruct) {
    vKey.push_back(keyStruct);
}

int PrimaryKey::Find(std::string key) {
    for (auto& e : vKey)
        if (key == e.key)
            return e.offset;

    return -1;
}