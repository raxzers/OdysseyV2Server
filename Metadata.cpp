//
// Created by raxzers on 6/4/18.
//

#include "Metadata.h"
void Metadata::fromJSON(boost::property_tree::ptree json) {


    for(boost::property_tree::ptree::value_type const& v : json){
        if (v.first == "name"){
            this->name = v.second.data();
        }
        else if(v.first == "artist"){
            this->artist = v.second.data();
        }
        else if(v.first == "album"){
            this->album = v.second.data();
        }
        else if(v.first == "pathName"){
            this->pathName = v.second.data();
        }
        else if(v.first == "lyrics"){
            this->lyrics = v.second.data();
        }
        else if(v.first == "genre"){
            this->genre = v.second.data();
        }
        else if(v.first == "year"){
            this->year = v.second.data();
        }


    }
}


boost::property_tree::ptree Metadata::toJSON() {

    boost::property_tree::ptree json;
    json.put("name", this->name);
    json.put("album", this->album);
    json.put("artist", this->artist);
    json.put("pathName", this->pathName);
    json.put("lyrics",this->lyrics);
    json.put("genre", this->genre);
    json.put("year",this->year);
    return json;
}

boost::property_tree::ptree Metadata::toXML() {

    boost::property_tree::ptree xml;
    xml.put("name", this->name);
    xml.put("album", this->album);
    xml.put("artist", this->artist);
    xml.put("lyrics", this->lyrics);
    xml.put("genre", this->genre);
    xml.put("year",this->year);
    return xml;
}

std::string Metadata::get(std::string param){
    if(param == "name"){
        return boost::algorithm::to_lower_copy(this->name);
    }
    else if(param == "album"){
        return boost::algorithm::to_lower_copy(this->album);
    }
    else if(param == "artist"){
        return boost::algorithm::to_lower_copy(this->artist);
    }
    else if(param == "genre"){
        return boost::algorithm::to_lower_copy(this->genre);
    }
    else if(param == "year"){
        return boost::algorithm::to_lower_copy(this->year);
    }
}