//
// Created by raxzers on 6/5/18.
//

#include "Sorter.h"
void Sorter::swap(std::vector<Metadata*> & arr, int i,int j)
{
    Metadata* temp = arr.at(i);
    arr.at(i) = arr.at(j);
    arr.at(j) = temp;

}

int Sorter::partition (std::vector<Metadata*> & arr, int low, int high)
{
    Metadata* pivot = arr.at(high);    // pivot
    int i = (low - 1);  // Index of smaller element

    for (int j = low; j <= high- 1; j++)
    {
        // If current element is smaller than or
        // equal to pivot
        if (arr.at(j)->get(ServerHandler::sortBy) <= pivot->get(ServerHandler::sortBy))
        {
            i++;    // increment index of smaller element
            swap(arr, i, j);
        }
    }
    swap(arr,i + 1,high);
    return (i + 1);
}

/* The main function that implements QuickSort
 arr[] --> Array to be sorted,
  low  --> Starting index,
  high  --> Ending index */
void Sorter::quickSort(std::vector<Metadata*>& arr, int low, int high)
{
    if (low < high)
    {
        /* pi is partitioning index, arr[p] is now
           at right place */
        int pi = partition(arr, low, high);

        // Separately sort elements before
        // partition and after partition
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// A function to implement bubble sort
void Sorter::bubbleSort(std::vector<Metadata*>& arr, int n)
{
    int i, j;
    for (i = 0; i < n-1; i++)

        // Last i elements are already in place
        for (j = 0; j < n-i-1; j++)
            if (arr[j]->get(ServerHandler::sortBy) > arr[j+1]->get(ServerHandler::sortBy))
                swap(arr, j,j+1);
}

size_t Sorter::getMax(std::vector<Metadata*> &arr, int n){
    size_t max = arr[0]->get(ServerHandler::sortBy).size();
    for (int i = 1; i < n; i++){
        if (arr[i]->get(ServerHandler::sortBy).size()>max)
            max = arr[i]->get(ServerHandler::sortBy).size();
    }
    return max;
}

void Sorter::countSort(std::vector<Metadata*> &a, int size, size_t k){
    Metadata** b = nullptr;
    int *c = nullptr;
    c = new int[257];
    b = new Metadata*[size];


    for (int i = 0; i <257; i++){
        c[i] = 0;
        //cout << c[i] << "\n";
    }
    for (int j = 0; j <size; j++){
        c[k < a[j]->get(ServerHandler::sortBy).size() ? (int)(unsigned char)a[j]->get(ServerHandler::sortBy)[k] + 1 : 0]++;            //a[j] is a string
        //cout << c[a[j]] << endl;
    }

    for (int f = 1; f <257; f++){
        c[f] += c[f - 1];
    }

    for (int r = size - 1; r >= 0; r--){
        b[c[k < a[r]->get(ServerHandler::sortBy).size()  ? (int)(unsigned char)a[r]->get(ServerHandler::sortBy)[k] + 1 : 0] - 1] = a[r];
        c[k < a[r]->get(ServerHandler::sortBy).size()  ? (int)(unsigned char)a[r]->get(ServerHandler::sortBy)[k] + 1 : 0]--;
    }
    for (int l = 0; l < size; l++){
        a[l] = b[l];
    }

    // avold memory leak
    delete[] b;
    delete[] c;
}
void Sorter::radixSort(std::vector<Metadata*>& b, int r){
    size_t max = getMax(b, r);
    for (size_t digit = max; digit > 0; digit--){ // size_t is unsigned, so avoid using digit >= 0, which is always true
        countSort(b, r, digit - 1);
    }

}

/* Function to print an array */
void Sorter::printArray(std::vector<Metadata*> arr, int size)
{
    int i;
    for (i=0; i < size; i++)
        std::cout<<arr[i]->get(ServerHandler::sortBy) <<std::endl;
}