//
// Created by raxzers on 6/4/18.
//
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/algorithm/string.hpp>
#ifndef ODYSSEYSERVER2_0_METADATA_H
#define ODYSSEYSERVER2_0_METADATA_H


class Metadata {
public:
    std::string name = "";
    std::string artist = "";
    std::string album  = "";
    std::string pathName = "";
    std::string lyrics = "";
    std::string genre = "";
    std::string year = "";

    /**carga un json en el objeto
    *
    * @param json
    */
    void fromJSON(boost::property_tree::ptree json);

    /**crea un json a partir del objeto
    *
    * @return arbol que representa el json
    */
    boost::property_tree::ptree toJSON();

    /**crea un xml a partir del objeto
    *
    * @return arbol que representa el xml
    */
    boost::property_tree::ptree toXML();


    std::string get(std::string param);
};


#endif //ODYSSEYSERVER2_0_METADATA_H
