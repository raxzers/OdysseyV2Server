//
// Created by raxzers on 6/2/18.
//

#include "ServerHandler.h"
#include "Sorter.h"

const std::string ServerHandler::trackPath  ="/home/raxzers/Desktop/Odyssey++/Tracks/";

const std::string ServerHandler::metadataPath  ="/home/raxzers/Desktop/Odyssey++/metadata.json";

const std::string ServerHandler::metadataTemplate  ="{\"users\" : [], \n  \"songs\" : []}";

int ServerHandler::NumberOfSongs = 0;

std::vector<Metadata*> ServerHandler::songs;

std::vector<Metadata*> ServerHandler::sortedAlbums;




const int ServerHandler::pageSize = 10;

const int ServerHandler::chunkSize = 327680;

std::string ServerHandler::sortBy = "name";

void ServerHandler::updateUsers(){
    //aqui va la actualiza los de usuarios en la base de datos
}



void ServerHandler::loadUsers() {
   //aqui va la carga de usuarios de la base de datos
}

void ServerHandler::loadSongs() {


}

void ServerHandler::loadSetUp(){

    loadUsers();
    loadSongs();
}

void ServerHandler::updateSongs(){

}


void ServerHandler::quickSort(){
    Sorter::quickSort(songs,0,NumberOfSongs-1);
}

void ServerHandler::bubbleSort(){
    Sorter::bubbleSort(songs,NumberOfSongs);
}

void ServerHandler::radixSort(){
    Sorter::radixSort(songs,NumberOfSongs);
}

void ServerHandler::insertAlbum(Metadata* insert){
    int i = 0;
    for(Metadata* song: sortedAlbums){
        if(song->album > insert->album){
            break;
        }
        i++;
    }
    sortedAlbums.insert(sortedAlbums.begin()+i,insert);
}


