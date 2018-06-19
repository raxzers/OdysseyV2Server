//
// Created by karina on 29/05/18.
//

#ifndef DATABASE_ODYSSEY_QUERY_H
#define DATABASE_ODYSSEY_QUERY_H


#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include <cstdio>
#include "rapidjson/filewritestream.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include "../DataStructures/SimpleLinkedList.h"

using namespace rapidjson;

class Query {
public:
    /*void readFile(std::string path);
    void findValue(std::string);
    bool checkCredentials(std::string user, std::string password);
    SimpleLinkedList<Metadata> getAllTracks();
    SimpleLinkedList<Metadata> getTrackByArtist(std::string artist);
    SimpleLinkedList<Metadata> getTrackByName(std::string name);
    SimpleLinkedList<Userdata> getUsers();
    SimpleLinkedList<std::string> getUserFriends(std::string username);
    SimpleLinkedList<std::string> getUserPreferences(std::string username);
    void addNewUser(Userdata userdata);
    void addNewTrack(Metadata metadata);*/

    void addTrack(std::string str1path, std::string str2path,
                  std::string str3path, std::string paritypath, std::string name, long stripe);
    void deleteTrack(std::string name);
    SimpleLinkedList<std::string> getPath(std::string name);
    long getStripeSize(std::string name);

private:
const std::string MAP_PATH="/home/karina/Escritorio/Tracks/DriveMap.json";
};


#endif //DATABASE_ODYSSEY_QUERY_H
