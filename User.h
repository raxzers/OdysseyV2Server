//
// Created by raxzers on 6/4/18.
//

#ifndef ODYSSEYSERVER2_0_USER_H
#define ODYSSEYSERVER2_0_USER_H

#include <iostream>
#include <vector>
#include <boost/property_tree/ptree.hpp>
class User {
private:
    std::string firstName;
    std::string lastName;
    std::string username;
    std::string password;
    std::vector<std::string> genres;
    std::vector<std::string> friends;
    int age;

    boost::property_tree::ptree GetJSONArray(std::vector<std::string> data);

public:
    const std::string &getFirstName() const;

    void setFirstName(const std::string &firstName);

    const std::string &getLastName() const;

    void setLastName(const std::string &lastName);

    const std::string &getUsername() const;

    void setUsername(const std::string &username);

    const std::string &getPassword() const;

    void setPassword(const std::string &password);

    const std::vector<std::string> &getGenres() const;

    void setGenres(const std::vector<std::string> &genres);

    const std::vector<std::string> &getFriends() const;

    void setFriends(const std::vector<std::string> &friends);

    int getAge() const;

    void setAge(int age);

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

};


#endif //ODYSSEYSERVER2_0_USER_H
