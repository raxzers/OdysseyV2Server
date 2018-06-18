//
// Created by karina on 13/06/18.
//

#include <fstream>
#include <cmath>
#include <iostream>
#include <random>
#include "MemoryController.h"
#include "../base64.h"
#include <malloc.h>

void MemoryController::write(std::string data, int disk) {
    std::ofstream trackFile;
    trackFile.open(trackPath + "Track9", std::ios::out);
    std::string rawData = base64_decode(data);
    trackFile.write(rawData.data(), rawData.size());
    trackFile.close();
}

std::string MemoryController::read(int diskNum) {
    std::fstream inFile;
    inFile.open(trackPath + "Track1.mp3", std::ios::in | std::ios::binary);

    int seek = 0;
    int bytes = 0;

    inFile.seekg(0, std::ios::end);
    int size = inFile.tellg();
    std::cout << "Size of file: " << size << std::endl;
    inFile.seekg(0, std::ios::beg);


    char *data = new char[(int)std::ceil((double)size/(double)4)];

    bytes = (int)std::ceil((double)size/(double)4) - seek;
    inFile.read(data, bytes);

    inFile.close();

    std::string strData(data, bytes);

    unsigned char newData[strData.size()];
    std::copy(strData.begin(), strData.end(), newData);
    newData[strData.length()] = 0;

    strData = base64_encode(newData, strData.size());


    return strData;
}

void MemoryController::seek(std::string track) {

}

void MemoryController::upload(std::string name) {
    std::ifstream inFile;
    inFile.open(trackPath + "sample2.mp3", std::ios::in | std::ios::binary);

    inFile.seekg(0, std::ios::end);
    long size = inFile.tellg();

    long stripeSize = (long) (std::ceil((double) size / (double) 3));

    std::cout << "file size: " << size << " stripe size: " << stripeSize << std::endl;

    char *stripe1 = (char *) malloc(sizeof(char) * size + 1);

    inFile.seekg(0, std::ios::beg);
    inFile.read(stripe1, size);
    inFile.close();

    char *parityStripe = getParity(stripe1, stripe1 + stripeSize, stripe1 + (2*stripeSize), stripeSize);

    bool available[] = {true, true, true, true};
    int stripesWriten[4];
    int disksAvailable = 4;
    std::cout << "before while" << std::endl;

    while (disksAvailable > 0) {
        std::mt19937 rng;
        rng.seed(std::random_device()());
        std::uniform_int_distribution<std::mt19937::result_type> dist6(0, 3); // distribution in range [1, 6]
        int disk = dist6(rng);
        std::string path = trackPath;
        path.append("Disk ");
        std::cout << "path before mess: " << path << std::endl;

        if (available[disk]) {
            std::cout << "selected disk to upload: " << disk  << std::endl;
            std::ofstream trackFile;
            std::string strData;
            std::string rawData;
            path.append(std::to_string(disk) + "/");
            std::cout << "path: " << path << std::endl;
            if (stripesWriten[0] != 1 && stripesWriten[1] != 1 && stripesWriten[2] != 1 && stripesWriten[3] != 1) {
                path.append(name + "-1.mp3");
                std::cout << "uploading stripe 1 to path: " << path << std::endl;

                //Stripe1
                unsigned char newData[stripeSize + 1];
                strData = std::string(stripe1, stripeSize);
                std::copy(strData.begin(), strData.end(), newData);
                newData[strData.length()] = 0;
                strData = base64_encode(newData, strData.size());

                trackFile.flush();
                trackFile.open(path, std::ios::out | std::ios::binary);
                rawData = base64_decode(strData);
                trackFile.write(rawData.data(), rawData.size());
                trackFile.close();

                disksAvailable--;
                stripesWriten[disk] = 1;
                available[disk] = false;
            } else if (stripesWriten[0] != 2 && stripesWriten[1] != 2 && stripesWriten[2] != 2 &&
                       stripesWriten[3] != 2) {
                path.append(name + "-2.mp3");
                std::cout << "uploading stripe 2 to path: " << path << std::endl;

                //Stripe2
                unsigned char newData2[stripeSize + 1];
                strData = std::string((stripe1 + stripeSize), stripeSize);
                std::copy(strData.begin(), strData.end(), newData2);
                newData2[strData.length()] = 0;
                strData = base64_encode(newData2, strData.size());

                trackFile.flush();
                trackFile.open(path, std::ios::out | std::ios::binary);
                rawData = base64_decode(strData);
                trackFile.write(rawData.data(), rawData.size());
                trackFile.close();

                disksAvailable--;
                stripesWriten[disk] = 2;
                available[disk] = false;
            } else if (stripesWriten[0] != 3 && stripesWriten[1] != 3 && stripesWriten[2] != 3 &&
                       stripesWriten[3] != 3) {
                path.append(name + "-3.mp3");
                std::cout << "uploading stripe 3 to path: " << path << std::endl;

                //Stripe3
                strData = std::string((stripe1 + (2*stripeSize)), stripeSize);
                unsigned char newData3[stripeSize + 1];
                std::copy(strData.begin(), strData.end(), newData3);
                newData3[strData.length()] = 0;
                strData = base64_encode(newData3, strData.size());
                trackFile.open(path, std::ios::out | std::ios::binary);
                rawData = base64_decode(strData);
                trackFile.write(rawData.data(), rawData.size());
                trackFile.close();

                disksAvailable--;
                stripesWriten[disk] = 3;
                available[disk] = false;
            } else if (stripesWriten[0] != 4 && stripesWriten[1] != 4 && stripesWriten[2] != 4 &&
                       stripesWriten[3] != 4) {
                path.append(name + "-parity");
                std::cout << "uploading stripe 4 to path: " << path << std::endl;


                //Parity
                strData = std::string(parityStripe, stripeSize);
                unsigned char newData4[stripeSize + 1];
                std::copy(strData.begin(), strData.end(), newData4);
                newData4[strData.length()] = 0;
                strData = base64_encode(newData4, strData.size());
                trackFile.open(path, std::ios::out | std::ios::binary);
                rawData = base64_decode(strData);
                trackFile.write(rawData.data(), rawData.size());
                trackFile.close();

                disksAvailable--;
                stripesWriten[disk] = 4;
                available[disk] = false;
            }

        }
    }
    /*std::fstream inFile;
    inFile.open(trackPath + name + ".mp3", std::ios::in | std::ios::binary);

    int seek = 0;
    int bytes = 0;

    inFile.seekg(0, std::ios::end);
    int size = inFile.tellg();
    int stripeSize = (int)std::ceil((double)size/(double)3);
    char stripe1[stripeSize];
    char stripe2[stripeSize];
    char stripe3[stripeSize];
    char* parityStripe  = getParity(stripe1, stripe2, stripe3);

    std::cout << "Size of file: " << size << std::endl;
    inFile.seekg(0, std::ios::beg);


    char *data = new char[(int)std::ceil((double)size/(double)4)];

    bytes = (int)std::ceil((double)size/(double)4) - seek;
    inFile.read(data, bytes);

    inFile.close();

    std::string strData(data, bytes);

    unsigned char newData[strData.size()];
    std::copy(strData.begin(), strData.end(), newData);
    newData[strData.length()] = 0;

    strData = base64_encode(newData, strData.size());*/
}

char *MemoryController::getParity(char *stripe1, char *stripe2, char *stripe3, int size) {
    char* parity = (char*)malloc(size* sizeof(char));
    for(int i = 0; i < size; i++){
        *( parity + (sizeof(char)*i)) = ((*(stripe1 + (sizeof(char)*i)) ^ (*(stripe2 + (sizeof(char)*i)))) ^ (*(stripe3 + (sizeof(char)*i))));
    }
    return parity;
}

void MemoryController::checkFile(std::string name) {
    std::string stripe1path = "/home/karina/Escritorio/Tracks/Disk 1/prueba-1.mp3";
    std::string stripe2path = "/home/karina/Escritorio/Tracks/Disk 2/prueba-2.mp3";
    std::string stripe3path = "/home/karina/Escritorio/Tracks/Disk 0/prueba-3.mp3";
    std::string stripeParityPath = "/home/karina/Escritorio/Tracks/Disk 3/prueba-parity";

    long fileSize;
    std::ifstream inFile;

    inFile.open(stripe1path, std::ios::in | std::ios::binary);
    inFile.seekg(0, std::ios::end);
    fileSize = inFile.tellg();
    inFile.seekg(0, std::ios::beg);

    char *stripe1 = (char *) malloc(sizeof(char) * fileSize + 1);
    inFile.read(stripe1, fileSize);
    inFile.close();

    inFile.open(stripe2path, std::ios::in | std::ios::binary);
    inFile.seekg(0, std::ios::end);
    fileSize = inFile.tellg();
    inFile.seekg(0, std::ios::beg);
    char *stripe2 = (char *) malloc(sizeof(char) * fileSize + 1);
    inFile.read(stripe2, fileSize);
    inFile.close();

    inFile.open(stripe3path, std::ios::in | std::ios::binary);
    inFile.seekg(0, std::ios::end);
    fileSize = inFile.tellg();
    inFile.seekg(0, std::ios::beg);
    char *stripe3 = (char *) malloc(sizeof(char) * fileSize + 1);
    inFile.read(stripe3, fileSize);
    inFile.close();

    inFile.open(stripeParityPath, std::ios::in | std::ios::binary);
    inFile.seekg(0, std::ios::end);
    fileSize = inFile.tellg();
    inFile.seekg(0, std::ios::beg);
    char *stripeParity = (char *) malloc(sizeof(char) * fileSize + 1);
    inFile.read(stripeParity, fileSize);
    inFile.close();

    bool available[] = {true, true, true, true};


    std::ofstream trackFile;
    std::string strData;
    std::string rawData;

    char *recoveryStripe = (char *) malloc(sizeof(char) * fileSize + 1);
   // char num = name.at(name.length() - 5);

    for (int i = 0; i < fileSize; i++) {
        *(recoveryStripe + i) = ((*(stripe1 + i)) ^ (*(stripe3 + i))) ^ (*(stripeParity + i));
    }

    unsigned char newData[fileSize + 1];
    strData = std::string(recoveryStripe, fileSize);
    std::copy(strData.begin(), strData.end(), newData);
    newData[strData.length()] = 0;
    strData = base64_encode(newData, strData.size());

    trackFile.flush();
    trackFile.open("/home/karina/Escritorio/Tracks/recovery.mp3", std::ios::out | std::ios::binary);
    rawData = base64_decode(strData);
    trackFile.write(rawData.data(), rawData.size());
    trackFile.close();

}
