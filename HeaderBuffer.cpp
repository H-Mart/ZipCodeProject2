#include "HeaderBuffer.h"

#include <iostream>
// #include <string>

void HeaderBuffer::read(std::istream& ins) {
    ins.seekg(0);

    HeaderInfo hInfo;
    ins >> hInfo;

    buffer.resize(hInfo.headerSize);
    ins.seekg(0);

    char c;
    for (int i = 0; i < hInfo.headerSize; i++) {
        ins.read(&c, 1);
        buffer[i] = c;
    }
}

Header HeaderBuffer::unpack() {
    Header header;
    std::vector<FieldInfo> fields;

    auto fileInfoOffset = sizeof(Header::headerInfo);

    memcpy(&header.headerInfo, &buffer[0], sizeof(HeaderInfo));
    memcpy(&header.fileInfo, &buffer[fileInfoOffset], sizeof(FileInfo));
    
    size_t fieldInfoOffset = fileInfoOffset + sizeof(Header::fileInfo);
    for (int i = 0; i < header.fileInfo.fieldsPerRecord; i++) {
        FieldInfo fieldInfo;
        memcpy(&fieldInfo, &buffer[fieldInfoOffset], sizeof(FieldInfo));
        fields.push_back(fieldInfo);

        // set offset to beginning of next field info
        fieldInfoOffset += sizeof(FieldInfo);
    }

    header.fields = fields;

    return header;
}
