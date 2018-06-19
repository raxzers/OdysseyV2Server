//
// Created by karina on 13/06/18.
//

#ifndef ODYSSEYSERVER2_0_MEMORYCONTROLLER_H
#define ODYSSEYSERVER2_0_MEMORYCONTROLLER_H


#include <string>

class MemoryController {

public:
    void write(std::string data, std::string path);
    std::string read(std::string path, int size, int pos);
    std::string seek(std::string track, int pos);
    void upload(std::string name, std::string pathToFile);
    char* getParity(char* stripe1, char* stripe2, char* stripe3, int size);
    void checkFile(std::string name);

private:

    const std::string trackPath  ="/home/karina/Escritorio/Tracks/";
    const int chunk = 10000;
};


#endif //ODYSSEYSERVER2_0_MEMORYCONTROLLER_H
