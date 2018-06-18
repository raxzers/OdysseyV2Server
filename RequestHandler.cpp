//
// Created by raxzers on 6/4/18.
//

#include "RequestHandler.h"
#include <set>
#include "Sorter.h"
#include <math.h>
std::string RequestHandler::handle(boost::property_tree::ptree xmlRequest){
    boost::property_tree::ptree response;
    std::string opCode = "-1";
    for(boost::property_tree::ptree::value_type const& v : xmlRequest.get_child("request")){
        if (v.first != "<xmlattr>"){
            continue;
        }
        opCode = v.second.get<std::string>("opcode");
        if(opCode == "1"){
            response = handleLogIn(xmlRequest);
        }
        else if(opCode == "2"){
            response = handleRegistration(xmlRequest);
        }
        else if(opCode == "3"){
            response = handleUpload(xmlRequest);
        }
        else if(opCode == "4"){
            response = handleSongLibrary(xmlRequest);
        }
        else if(opCode == "5"){
            response = handlePlay(xmlRequest);
        }
        else if(opCode == "6"){
            response = handleSearch(xmlRequest);
        }
        else if(opCode == "7"){
            response = handleLyricsGuessing(xmlRequest);
        }
        else if(opCode == "8"){
            response = handleDeletion(xmlRequest);
        }
        else if(opCode == "9"){
            response = handleChangeMetadata(xmlRequest);
        }

    }
    boost::property_tree::ptree responseXML;
    responseXML.put_child("response",response);
    responseXML.put("response.<xmlattr>.opcode",opCode);
    std::ostringstream stream;
    boost::property_tree::write_xml(stream,responseXML);
    return stream.str();

}

boost::property_tree::ptree RequestHandler::handleLogIn(boost::property_tree::ptree xmlRequest){
    std::cout<< "Log In Request" <<std::endl;
    User* user = nullptr;
    boost::property_tree::ptree response;

    for(boost::property_tree::ptree::value_type const& v : xmlRequest.get_child("request")) {
        if (v.first == "username") {
            user = ServerHandler::users->get(v.second.data());
            if (user == nullptr) {
                response.put("error",false);
                response.put("description","invalid username or password");
                response.put("status",false);
                break;
            }
        }else if(v.first == "password"){
            if(user == nullptr){
                response.put("error",true);
            }
            else if (user->getPassword() == encryptPassword(v.second.data())) {
                response.put("error",false);
                response.put("description","access granted");
                response.put("status",true);
            }else{
                response.put("error",false);
                response.put("description","invalid username or password");
                response.put("status",false);
            }
            break;
        }
    }
    return response;

}

boost::property_tree::ptree RequestHandler::handleRegistration(boost::property_tree::ptree xmlRequest){
    std::cout<< "Registration Request" <<std::endl;
    User* newUser = new User();
    for(boost::property_tree::ptree::value_type const& v : xmlRequest.get_child("request")){
        if (v.first == "first_name"){
            newUser->setFirstName(v.second.data());
        }
        else if(v.first == "last_name"){
            newUser->setLastName(v.second.data());
        }
        else if(v.first == "username"){
            newUser->setUsername(v.second.data());
        }
        else if(v.first == "password"){
            std::string encryptedPassword = encryptPassword(v.second.data());
            newUser->setPassword(encryptedPassword);
        }
        else if(v.first == "age"){
            newUser->setAge(std::stoi(v.second.data()));
        }
        else if(v.first == "genres"){
            std::vector<std::string> genres;
            for (auto& item : xmlRequest.get_child("genres"))
                genres.push_back(item.second.data());
            newUser->setGenres(genres);
        }
        else if(v.first == "friends"){
            std::vector<std::string> friends;
            for (auto& item : xmlRequest.get_child("friends"))
                friends.push_back(item.second.data());
            newUser->setFriends(friends);
        }
    }

    boost::property_tree::ptree response;
    response.put("error",false);

    if(!ServerHandler::users->insert(newUser)){//revisa si el usuario existe
        response.put("description","username already exists");
        response.put("status",false);
    }else{
        response.put("description","successful registration");
        response.put("status",true);
    }
    ServerHandler::updateUsers();
    return response;
}

boost::property_tree::ptree RequestHandler::handleUpload(boost::property_tree::ptree xmlRequest){
    std::cout<< "Upload Request" <<std::endl;
    std::ofstream trackFile;
    ServerHandler::NumberOfSongs ++;
    trackFile.open(ServerHandler::trackPath +"Track"+ std::to_string(ServerHandler::NumberOfSongs), std::ios::out);
    Metadata* song = new Metadata();
    song->pathName = "Track"+std::to_string(ServerHandler::NumberOfSongs);
    for(boost::property_tree::ptree::value_type const& v : xmlRequest.get_child("request")){
        if(v.first == "name"){
            song->name = v.second.data();
        }
        else if(v.first == "artist"){
            song->artist = v.second.data();
        }
        else if(v.first == "album"){
            song->album = v.second.data();
        }
        else if( v.first == "content") {
            std::string rawData = base64_decode(v.second.data());
            trackFile.write(rawData.data(), rawData.size());
        }else if(v.first == "lyrics"){
            song->lyrics = v.second.data();
        }else if(v.first == "genre"){
            song->genre = v.second.data();
        }else if(v.first == "year"){
            song->year = v.second.data();
        }
    }
    std::vector<Metadata*> songs;
   // songs = ServerHandler::songsNames->search(song->name);

    boost::property_tree::ptree response;

    if (songs.size() != 0){
        while(true){
            for(Metadata* data :songs){
                if(data->album == song->album && data->artist == song->artist && data->lyrics == song->lyrics){
                    ServerHandler::NumberOfSongs--;
                    trackFile.close();
                    std::remove((ServerHandler::trackPath+song->pathName).data());

                    response.put("error",false);
                    response.put("description","song already uploaded");
                    response.put("status",false);

                    delete(song);

                    return response;
                }
            }
            break;
        }
    }

    //ServerHandler::songsNames->insert(song);
    //ServerHandler::songsArtists->insert(song);
    ServerHandler::songs.push_back(song);
    ServerHandler::insertAlbum(song);
    ServerHandler::updateSongs();
    trackFile.close();


    response.put("error",false);
    response.put("description","successful upload");
    response.put("status",true);

    return response;
}

std::string RequestHandler::encryptPassword(std::string password){
    int size = password.size();
    int ascii[size];
    int index = 0;
    int sum = 0;
    for(char character : password){
        ascii[index] = int(character);
        index++;
        sum+= int(character);
    }
    int offset = size % sum;
    std::string response = "";
    for(index = 0; index < size; index++){
        char asciiCharacter= ascii[index] + offset;
        response += asciiCharacter;
    }
    char salt[size*3];
    generateSalt(salt,size*3);
    unsigned char data[response.size()];
    std::copy( response.begin(), response.end(), data);
    data[response.length()] = 0;
    response = base64_encode(data,response.size());
    //response.append(salt);
    return response;
}

void RequestHandler::generateSalt(char *buffer, const int size) {
    static const char alphanum[] =
            "0123456789"
                    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                    "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < size; ++i) {
        buffer[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    buffer[size] = 0;
}

boost::property_tree::ptree RequestHandler::handleSongLibrary(boost::property_tree::ptree xmlRequest){
    std::cout<< "Library Request" <<std::endl;
    boost::property_tree::ptree responseXML;
    for(boost::property_tree::ptree::value_type const& v : xmlRequest.get_child("request")){
        if(v.first == "sortBy"){
            ServerHandler::sortBy = v.second.data();
        }
        else if(v.first == "sortWith"){
            if(v.second.data() == "quickSort"){
                ServerHandler::quickSort();
            }
            else if(v.second.data() == "bubbleSort"){
                ServerHandler::bubbleSort();
            }
            else if(v.second.data() == "radixSort"){
                ServerHandler::radixSort();
            }
        }
        else if(v.first == "page"){
            responseXML = getSongs(std::atoi(v.second.data().data()));
        }
    }
    return responseXML;
}

boost::property_tree::ptree RequestHandler::getSongs(int page){
    int index = 0;
    int songs = 0;

    if(page != 0){
        index = page*ServerHandler::pageSize;
    }
    boost::property_tree::ptree tree;
    boost::property_tree::ptree parent;
    for(int i = 0; i < ServerHandler::pageSize;i++){
        if (index >= ServerHandler::songs.size()){
            break;
        }
        tree.push_back(std::make_pair("song",ServerHandler::songs.at(index)->toXML()));
        index++;
        songs++;
    }

    int pages = (int)ceil((double)ServerHandler::songs.size()/(double)ServerHandler::pageSize);
    parent.put("numberOfSongs",songs);
    parent.put("pages",pages);
    parent.put("pageSize",ServerHandler::pageSize);
    parent.add_child("songs",tree);

    boost::property_tree::write_xml(std::cout,parent);
    return parent;

}


boost::property_tree::ptree RequestHandler::handlePlay(boost::property_tree::ptree xmlRequest){
    std::cout<< "Play Request" <<std::endl;
    boost::property_tree::ptree responseXML;
    Metadata* song = nullptr;
    std::string name = "";
    std::string album = "";
    std::string artist = "";
    std::string genre = "";
    std::string year = "";
    //std::string lyrics = " ";
    for(boost::property_tree::ptree::value_type const& v : xmlRequest.get_child("request")){
        if(v.first == "name"){
            name = v.second.data();
        }else if(v.first == "album"){
            album = v.second.data();
        }else if(v.first == "artist"){
            artist = v.second.data();
        }/*else if(v.first == "lyrics"){
            lyrics = v.second.data();
        }*/else if(v.first == "genre"){
            genre = v.second.data();
        }else if(v.first == "year"){
            year = v.second.data();
        }
        else if(v.first == "chunk"){
            std::vector<Metadata*> songs;
            songs = ServerHandler::songsNames->search(name);//busca la cancion
            for(Metadata* data :songs){
                if(data->album == album && data->artist == artist /*&& data->genre == genre*/ && data->year == year){ //&& data->lyrics == lyrics
                    song = data;
                    break;
                }
            }
            if (song == nullptr){
                responseXML.put("error",true);
                responseXML.put("description","song doesn't exist");
                break;
            }
            std::cout<<song->name<<std::endl;
            responseXML = getChunk(song->pathName,std::atoi(v.second.data().data()));
            responseXML.put("error",false);
            responseXML.put("description","success");
            break;
        }
    }
    boost::property_tree::write_xml(std::cout,responseXML);
    return responseXML;
}

boost::property_tree::ptree RequestHandler::getChunk(std::string path,int chunk){

    std::fstream trackFile;
    trackFile.open(ServerHandler::trackPath +path, std::ios::in|std::ios::binary);

    if(!trackFile){}

    int seek = 0;
    int bytes = 0;
    if(chunk != 0){
        seek = chunk*ServerHandler::chunkSize;
    }

    trackFile.seekg(0, std::ios::end);
    int size = trackFile.tellg();
    trackFile.seekg(0, std::ios::beg);

    if(seek != 0) {trackFile.seekg(seek);}

    char* data = new char[ServerHandler::chunkSize];

    if(seek + ServerHandler::chunkSize > size){
        bytes = size-seek;
        trackFile.read(data,bytes);

    }else{
        bytes = ServerHandler::chunkSize;
        trackFile.read(data,bytes);
    }
    trackFile.close();

    boost::property_tree::ptree parent;
    int chunks = (int)std::ceil((double)size/(double)ServerHandler::chunkSize);


    std::string strData(data, bytes);

    unsigned char newData[strData.size()];
    std::copy( strData.begin(), strData.end(), newData);
    newData[strData.length()] = 0;

    strData = base64_encode(newData,strData.size());
    parent.put("content",strData);
    parent.put("NumberOfBytes",bytes);
    parent.put("chunks",chunks);
    parent.put("chunkSize",ServerHandler::chunkSize);

    delete[] data;
    return parent;

}

boost::property_tree::ptree RequestHandler::handleSearch(boost::property_tree::ptree xmlRequest){
    std::cout<< "Search Request" <<std::endl;
    boost::property_tree::ptree responseXML;
    std::set<Metadata*> searchedSongs;
    for(boost::property_tree::ptree::value_type const& v : xmlRequest.get_child("request")){
        if(v.first == "search") {
            std::string search = v.second.data();
            backTrack(search,searchedSongs);
            for (Metadata *data : ServerHandler::songsNames->search(search)) {
                searchedSongs.insert(data);
            }
            for (Metadata *data : ServerHandler::songsArtists->search(search)) {
                searchedSongs.insert(data);
            }
            for (Metadata *data : ServerHandler::searchAlbums(search)) {
                searchedSongs.insert(data);
            }

        }
    }
    int i = 0;
    boost::property_tree::ptree tree;
    for(Metadata* data :searchedSongs) {
        i++;
        tree.push_back(std::make_pair("song", data->toXML()));
    }

    responseXML.put("numberOfSongs",i);
    responseXML.add_child("songs",tree);

    return responseXML;
}

boost::property_tree::ptree RequestHandler::handleLyricsGuessing(boost::property_tree::ptree xmlRequest){
    std::cout<< "Lyrics Guessing Request" <<std::endl;
    boost::property_tree::ptree responseXML;
    std::string left = " ";
    std::string right = " ";
    for(boost::property_tree::ptree::value_type const& v : xmlRequest.get_child("request")){
        if(v.first == "left") {
            left = v.second.data();
            std::transform(left.begin(), left.end(), left.begin(), ::tolower);

        }else if(v.first == "right") {
            right = v.second.data();
            std::transform(right.begin(), right.end(), right.begin(), ::tolower);
        }
    }

    std::regex population_regex(left + "(.*?)\\s" + right);
    std::set<std::string> population;
    for (Metadata *data : ServerHandler::songs) {
        std::string lyrics = data->lyrics;
        std::transform(lyrics.begin(), lyrics.end(), lyrics.begin(), ::tolower);
        std::replace(lyrics.begin(),lyrics.end(),'\n',' ');
        std::replace(lyrics.begin(),lyrics.end(),',',' ');
        auto words_begin = std::sregex_iterator(lyrics.begin(), lyrics.end(), population_regex);
        auto words_end = std::sregex_iterator();
        for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
            std::string match_str = i->str();
            match_str.erase(0, left.length());
            match_str.erase(match_str.length() - right.length(), match_str.length());
            population.insert(match_str);
        }
    }
    boost::property_tree::ptree tree;
    int i = 0;
    for(std::string word :population) {
        i++;
        tree.add("word",word);
    }

    responseXML.put("numberOfWords",i);
    responseXML.add_child("words",tree);

    return responseXML;

}


boost::property_tree::ptree RequestHandler::handleDeletion(boost::property_tree::ptree xmlRequest){
    std::cout<< "Deletion Request" <<std::endl;
    boost::property_tree::ptree responseXML;

    Metadata* song = nullptr;
    std::string name = "";
    std::string album = "";
    std::string artist = "";
    std::string genre = "";
    std::string year = "";
    //std::string lyrics = " ";
    for(boost::property_tree::ptree::value_type const& v : xmlRequest.get_child("request")) {
        if (v.first == "name") {
            name = v.second.data();
        } else if (v.first == "album") {
            album = v.second.data();
        } else if (v.first == "artist") {
            artist = v.second.data();
        }/*else if(v.first == "lyrics"){
            lyrics = v.second.data();
        }*/else if (v.first == "genre") {
            genre = v.second.data();
        } else if (v.first == "year") {
            year = v.second.data();
        }
    }

    std::vector<Metadata*> songs = ServerHandler::songsNames->search(name);

    for(Metadata* data :songs){
        if(data->album == album && data->artist == artist /*&& data->genre == genre*/ && data->year == year){ //&& data->lyrics == lyrics
            song = data;
            break;
        }
    }
    if (song == nullptr){
        responseXML.put("error",true);
        responseXML.put("description","song doesn't exist");
        return responseXML;
    }

    std::remove((ServerHandler::trackPath + song->pathName).data());

    ServerHandler::songs.erase(std::remove(ServerHandler::songs.begin(), ServerHandler::songs.end(), song), ServerHandler::songs.end());
    delete(song);
    ServerHandler::updateSongs();
    for(Metadata* data :ServerHandler::songs){
        delete(data);
    }

    ServerHandler::sortedAlbums.clear();
    //ServerHandler::songsArtists->empty();
    //ServerHandler::songsNames->empty();
    ServerHandler::loadSetUp();

    responseXML.put("error",false);
    responseXML.put("description","song deleted");

    return responseXML;
}

void RequestHandler::backTrack(std::string temp,std::set<Metadata *> & searchedSongs){
    std::vector<std::string> phrase;
    std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
    split(temp,' ',phrase);

    int phraseSize = phrase.size();


    for (Metadata *data : ServerHandler::songs) {
        std::vector<std::string> population;
        std::string lyrics = data->lyrics;
        std::transform(lyrics.begin(), lyrics.end(), lyrics.begin(), ::tolower);
        std::replace(lyrics.begin(),lyrics.end(),'\n',' ');
        std::replace(lyrics.begin(),lyrics.end(),',',' ');
        split(lyrics,' ',population);
        int i = 0;
        for (std::string word : population) {
            if(i == phraseSize){
                searchedSongs.insert(data);
                break;
            }else if(word == phrase.at(i)){
                i++;
            }else{
                i = 0;
            }
        }
    }
}

boost::property_tree::ptree RequestHandler::handleChangeMetadata(boost::property_tree::ptree xmlRequest){
    std::cout<< "Change Metadata Request" <<std::endl;
    boost::property_tree::ptree responseXML;
    std::string name,album,artist,genre,year;

    std::string newName,newAlbum,newArtist,newGenre,newYear,newLyrics;

    for(boost::property_tree::ptree::value_type const& v : xmlRequest.get_child("request")){
        if(v.first == "name"){
            name = v.second.data();
        }else if(v.first == "album"){
            album = v.second.data();
        }else if(v.first == "artist"){
            artist = v.second.data();
        }/*else if(v.first == "lyrics"){
            lyrics = v.second.data();
        }*/else if(v.first == "genre"){
            genre = v.second.data();
        }else if(v.first == "year"){
            year = v.second.data();
        }else if(v.first == "newName"){
            newName = v.second.data();
        }else if(v.first == "newAlbum"){
            newAlbum = v.second.data();
        }else if(v.first == "newArtist"){
            newArtist = v.second.data();
        }else if(v.first == "newGenre"){
            newGenre = v.second.data();
        }else if(v.first == "newYear"){
            newYear = v.second.data();
        }else if(v.first == "newLyrics"){
            newLyrics = v.second.data();
        }
    }

    std::vector<Metadata*> songs = ServerHandler::songsNames->search(name);//busca la cancion
    Metadata* song = nullptr;
    for(Metadata* data: songs){
        if(data->album == album && data->artist == artist){ //&& data->lyrics == lyrics
            song = data;
            song->name = newName;
            song->album = newAlbum;
            song->artist = newArtist;
            song->genre = newGenre;
            song->year = newYear;
            song->lyrics = newLyrics;
            break;
        }
    }
    ServerHandler::updateSongs();
    responseXML.put("error",false);
    responseXML.put("description","Data changed successfully");
    boost::property_tree::write_xml(std::cout,responseXML);
    return responseXML;
}

void RequestHandler::split(const std::string &s, char delim, std::vector<std::string> &elems) {
    elems.clear();
    std::stringstream ss(s);
    std::string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
}