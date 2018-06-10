//
// Created by raxzers on 6/5/18.
//

#ifndef ODYSSEYSERVER2_0_SORTER_H
#define ODYSSEYSERVER2_0_SORTER_H

#include <vector>
#include "Metadata.h"
#include "ServerHandler.h"
class Sorter {
public:
    static void quickSort(std::vector<Metadata*>& songs,int first,int last);
    static void bubbleSort(std::vector<Metadata*>& songs,int size);
    static void radixSort(std::vector<Metadata*>& songs,int size);
    static void printArray(std::vector<Metadata*> arr, int size);

private:
    static size_t getMax(std::vector<Metadata*> &arr, int n);
    static void countSort(std::vector<Metadata*> &a, int size, size_t k);
    static void swap(std::vector<Metadata*> &arr,int i,int j);
    static int partition (std::vector<Metadata*> & arr, int low, int high);
};



#endif //ODYSSEYSERVER2_0_SORTER_H
