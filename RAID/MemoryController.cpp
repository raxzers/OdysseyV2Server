//
// Created by karina on 13/06/18.
//

#include <fstream>
#include <cmath>
#include <iostream>
#include <random>
#include "MemoryController.h"
#include "../base64.h"
#include "../JSON/Query.h"
#include "../DataStructures/SimpleLinkedList.h"
#include <malloc.h>

void MemoryController::write(std::string data, std::string path) {
    std::ofstream trackFile;
    trackFile.open(path, std::ios::out);
    std::string rawData = base64_decode(data);
    trackFile.write(rawData.data(), rawData.size());
    trackFile.close();
}

std::string MemoryController::read(std::string path, int size, int start) {
    std::fstream inFile;
    inFile.open(path, std::ios::in | std::ios::binary);
    inFile.seekg(start, std::ios::beg);
    char *data = new char[size + 1];
    inFile.read(data, size);

    inFile.close();

    std::string strData = base64_encode((const unsigned char *)data, size);
    return strData;
}

std::string MemoryController::seek(std::string track, int pos) {
    Query query;
    long stripeSize = query.getStripeSize(track);
    int pageToGet = (int) floor(pos / stripeSize);
    int position = pos % stripeSize;
    SimpleLinkedList<std::string> paths = query.getPath(track);
    auto temp = paths.getHead();
    for (int i = 0; i < pageToGet; i++) {
        temp = temp->getNext();
    }
    std::string path = temp->getData();
    return seek(path, position);

}

void MemoryController::upload(std::string name, std::string pathToFile) {
    std::ifstream inFile;
    inFile.open(pathToFile, std::ios::in | std::ios::binary);

    inFile.seekg(0, std::ios::end);
    long size = inFile.tellg();

    long stripeSize = (long) (std::ceil((double) size / (double) 3));

    char *stripe1 = (char *) malloc(sizeof(char) * size + 1);

    inFile.seekg(0, std::ios::beg);
    inFile.read(stripe1, size);
    inFile.close();

    char *parityStripe = getParity(stripe1, stripe1 + stripeSize, stripe1 + (2 * stripeSize), stripeSize);

    bool available[] = {true, true, true, true};
    int stripesWriten[4];
    int disksAvailable = 4;
    std::string paths[4];

    while (disksAvailable > 0) {
        std::mt19937 rng;
        rng.seed(std::random_device()());
        std::uniform_int_distribution<std::mt19937::result_type> dist6(0, 3); // distribution in range [1, 6]
        int disk = dist6(rng);
        std::string path = trackPath;
        path.append("Disk ");

        if (available[disk]) {
            std::cout << "selected disk to upload: " << disk << std::endl;
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

                write(strData, path);

                disksAvailable--;
                stripesWriten[disk] = 1;
                available[disk] = false;
                paths[0] = path;
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

                write(strData, path);

                disksAvailable--;
                stripesWriten[disk] = 2;
                available[disk] = false;
                paths[1] = path;
            } else if (stripesWriten[0] != 3 && stripesWriten[1] != 3 && stripesWriten[2] != 3 &&
                       stripesWriten[3] != 3) {
                path.append(name + "-3.mp3");
                std::cout << "uploading stripe 3 to path: " << path << std::endl;

                //Stripe3
                strData = std::string((stripe1 + (2 * stripeSize)), stripeSize);
                unsigned char newData3[stripeSize + 1];
                std::copy(strData.begin(), strData.end(), newData3);
                newData3[strData.length()] = 0;
                strData = base64_encode(newData3, strData.size());

                write(strData, path);

                disksAvailable--;
                stripesWriten[disk] = 3;
                available[disk] = false;
                paths[2] = path;
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


                write(strData, path);

                disksAvailable--;
                stripesWriten[disk] = 4;
                available[disk] = false;
                paths[3] = path;
            }


        }
    }
    Query query;
    query.addTrack(paths[0], paths[1], paths[2], paths[3], name, stripeSize);
}

char *MemoryController::getParity(char *stripe1, char *stripe2, char *stripe3, int size) {
    char* parity = (char*)malloc(size* sizeof(char));
    for(int i = 0; i < size; i++){
        *( parity + (sizeof(char)*i)) = ((*(stripe1 + (sizeof(char)*i)) ^ (*(stripe2 + (sizeof(char)*i)))) ^ (*(stripe3 + (sizeof(char)*i))));
    }
    return parity;
}

void MemoryController::checkFile(std::string name) {
    Query query;
    SimpleLinkedList<std::string> paths = query.getPath(name);
    std::string stripe1path = paths.getHead()->getData();
    std::string stripe2path = paths.getHead()->getNext()->getData();
    std::string stripe3path = paths.getHead()->getNext()->getNext()->getData();
    std::string stripeParityPath = paths.getHead()->getNext()->getNext()->getNext()->getData();

    long fileSize;
    std::ifstream inFile;
    bool available[] = {true, true, true, true};

    char* stripe1;
    char* stripe2;
    char* stripe3;
    char* stripeParity;


    inFile.open(stripe1path, std::ios::in | std::ios::binary);
    if(!inFile.fail()) {
        inFile.seekg(0, std::ios::end);
        fileSize = inFile.tellg();
        inFile.seekg(0, std::ios::beg);

        stripe1 = (char *) malloc(sizeof(char) * fileSize + 1);
        inFile.read(stripe1, fileSize);
    }else{
        available[0] = false;
    }
    inFile.close();

    inFile.open(stripe2path, std::ios::in | std::ios::binary);
    if(!inFile.fail()) {
        inFile.seekg(0, std::ios::end);
        fileSize = inFile.tellg();
        inFile.seekg(0, std::ios::beg);
        stripe2 = (char *) malloc(sizeof(char) * fileSize + 1);
        inFile.read(stripe2, fileSize);
    }else{
        available[1] = false;
    }
    inFile.close();

    inFile.open(stripe3path, std::ios::in | std::ios::binary);
    if(!inFile.fail()) {
        inFile.seekg(0, std::ios::end);
        fileSize = inFile.tellg();
        inFile.seekg(0, std::ios::beg);
        stripe3 = (char *) malloc(sizeof(char) * fileSize + 1);
        inFile.read(stripe3, fileSize);
    } else{
        available[2] = false;
    }
    inFile.close();

    inFile.open(stripeParityPath, std::ios::in | std::ios::binary);
    if(!inFile.fail()) {
        inFile.seekg(0, std::ios::end);
        fileSize = inFile.tellg();
        inFile.seekg(0, std::ios::beg);
        stripeParity = (char *) malloc(sizeof(char) * fileSize + 1);
        inFile.read(stripeParity, fileSize);
    }
    else{
        available[3] = false;
    }
    inFile.close();

    std::ofstream trackFile;
    std::string strData;
    std::string rawData;

    char *recoveryStripe = (char *) malloc(sizeof(char) * fileSize + 1);
    if(!available[0]){

        for (int i = 0; i < fileSize; i++) {
            *(recoveryStripe + i) = ((*(stripe2 + i)) ^ (*(stripe3 + i))) ^ (*(stripeParity + i));
        }
        unsigned char newData[fileSize + 1];
        strData = std::string(recoveryStripe, fileSize);
        std::copy(strData.begin(), strData.end(), newData);
        newData[strData.length()] = 0;
        strData = base64_encode(newData, strData.size());

        write(strData, stripe1path);

    }else if(!available[1]){

        for (int i = 0; i < fileSize; i++) {
            *(recoveryStripe + i) = ((*(stripe1 + i)) ^ (*(stripe3 + i))) ^ (*(stripeParity + i));
        }
        unsigned char newData[fileSize + 1];
        strData = std::string(recoveryStripe, fileSize);
        std::copy(strData.begin(), strData.end(), newData);
        newData[strData.length()] = 0;
        strData = base64_encode(newData, strData.size());

        write(strData, stripe2path);

    }else if(!available[2]){

        for (int i = 0; i < fileSize; i++) {
            *(recoveryStripe + i) = ((*(stripe1 + i)) ^ (*(stripe2 + i))) ^ (*(stripeParity + i));
        }
        unsigned char newData[fileSize + 1];
        strData = std::string(recoveryStripe, fileSize);
        std::copy(strData.begin(), strData.end(), newData);
        newData[strData.length()] = 0;
        strData = base64_encode(newData, strData.size());

        write(strData, stripe3path);

    } else if(!available[3]){
        for (int i = 0; i < fileSize; i++) {
            *(recoveryStripe + i) = ((*(stripe1 + i)) ^ (*(stripe2 + i))) ^ (*(stripe3 + i));
        }
        unsigned char newData[fileSize + 1];
        strData = std::string(recoveryStripe, fileSize);
        std::copy(strData.begin(), strData.end(), newData);
        newData[strData.length()] = 0;
        strData = base64_encode(newData, strData.size());

        write(strData, stripeParityPath);

    }
}
