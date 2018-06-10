//
// Created by raxzers on 6/4/18.
//

#ifndef ODYSSEYSERVER2_0_REQUESTHANDLER_H
#define ODYSSEYSERVER2_0_REQUESTHANDLER_H
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "User.h"
#include "ServerHandler.h"
#include <boost/property_tree/xml_parser.hpp>
#include "base64.h"
#include <regex>
#include <set>

class RequestHandler {
public:
    /**analiza las peticiones y atiende según su codigo
     *
     * @param xmlRequest
     * @return
     */
    static std::string handle(boost::property_tree::ptree xmlRequest);

    /**encripta un string mediante una función de hash
     *
     * @param password string por encriptar
     * @return string encriptado
     */
    static std::string encryptPassword(std::string password);
private:
    static boost::property_tree::ptree handleLogIn(boost::property_tree::ptree xmlRequest);
    static boost::property_tree::ptree handleRegistration(boost::property_tree::ptree xmlRequest);
    static boost::property_tree::ptree handleUpload(boost::property_tree::ptree xmlRequest);
    static boost::property_tree::ptree handleSongLibrary(boost::property_tree::ptree xmlRequest);
    static boost::property_tree::ptree handlePlay(boost::property_tree::ptree xmlRequest);
    static void generateSalt(char* buffer, int size);
    static boost::property_tree::ptree getSongs(int page);
    static boost::property_tree::ptree getChunk(std::string parh,int page);
    static boost::property_tree::ptree handleSearch(boost::property_tree::ptree xmlRequest);
    static boost::property_tree::ptree handleLyricsGuessing(boost::property_tree::ptree xmlRequest);
    static boost::property_tree::ptree handleDeletion(boost::property_tree::ptree xmlRequest);
    static void split(const std::string &s, char delim, std::vector<std::string> &elems);
    static boost::property_tree::ptree handleChangeMetadata(boost::property_tree::ptree xmlRequest);
    static void backTrack(std::string temp,std::set<Metadata *> & searchedSongs);
};


#endif //ODYSSEYSERVER2_0_REQUESTHANDLER_H
