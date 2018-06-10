//
// Created by raxzers on 6/4/18.
//

#include "User.h"
const std::string &User::getFirstName() const {
    return firstName;
}

void User::setFirstName(const std::string &firstName) {
    User::firstName = firstName;
}

const std::string &User::getLastName() const {
    return lastName;
}

void User::setLastName(const std::string &lastName) {
    User::lastName = lastName;
}

const std::string &User::getUsername() const {
    return username;
}

void User::setUsername(const std::string &username) {
    User::username = username;
}

const std::string &User::getPassword() const {
    return password;
}

void User::setPassword(const std::string &password) {
    User::password = password;
}

const std::vector<std::string> &User::getGenres() const {
    return genres;
}

void User::setGenres(const std::vector<std::string> &genres) {
    User::genres = genres;
}

const std::vector<std::string> &User::getFriends() const {
    return friends;
}

void User::setFriends(const std::vector<std::string> &friends) {
    User::friends = friends;
}

int User::getAge() const {
    return age;
}

void User::setAge(int age) {
    User::age = age;
}

void User::fromJSON(boost::property_tree::ptree json) {

    for(boost::property_tree::ptree::value_type const& v : json){
        if (v.first == "first_name"){
            this->firstName = v.second.data();
        }
        else if(v.first == "last_name"){
            this->lastName = v.second.data();
        }
        else if(v.first == "username"){
            this->username = v.second.data();
        }
        else if(v.first == "password"){
            this->password = v.second.data();
        }
        else if(v.first == "age"){
            this->age = std::stoi(v.second.data());
        }
        else if(v.first == "genres"){
            std::vector<std::string> genres;
            for (auto& item : json.get_child("genres"))
                genres.push_back(item.second.data());
            this->genres = genres;
        }
        else if(v.first == "friends"){
            std::vector<std::string> friends;
            for (auto& item : json.get_child("friends"))
                friends.push_back(item.second.data());
            this->friends = friends;
        }

    }
}

boost::property_tree::ptree User::toJSON() {
    boost::property_tree::ptree json;
    json.put("first_name", this->firstName);
    json.put("last_name", this->lastName);
    json.put("username", this->username);
    json.put("password", this->password);

    json.put("age", this->age);

    json.add_child("genres", GetJSONArray(this->genres));
    json.add_child("friends", GetJSONArray(this->friends));

    return json;
}

boost::property_tree::ptree User::GetJSONArray(std::vector<std::string> data){

    boost::property_tree::ptree children;

    for (auto &&specificData : data) {
        boost::property_tree::ptree child;

        child.put("",specificData);
        children.push_back(std::make_pair("",child));
    }

    return children;
}