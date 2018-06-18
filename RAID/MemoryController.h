//
// Created by karina on 13/06/18.
//

#ifndef ODYSSEYSERVER2_0_MEMORYCONTROLLER_H
#define ODYSSEYSERVER2_0_MEMORYCONTROLLER_H


#include <string>

class MemoryController {

public:
    void write(std::string data, int disk);
    std::string read(int diskNum);
    void seek(std::string track);
    void upload(std::string name);
    char* getParity(char* stripe1, char* stripe2, char* stripe3, int size);
    void checkFile(std::string name);

private:

    const std::string trackPath  ="/home/karina/Escritorio/Tracks/";
};


#endif //ODYSSEYSERVER2_0_MEMORYCONTROLLER_H
