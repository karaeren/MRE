/**
 *Here are the source codes of the functions created in the header file.
 * @file userDB.cpp
 * @author Abdülkadir Furkan Yıldız and Sefa Satıloğlu
 */

#include "userDb.hpp"

#include <string>
//frameworks
#include "oatpp/core/Types.hpp"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

/*
    Source Codes
    @param Voting, Delete, Rotate Movie / Series Functions
*/

//The function the user will score
bool UserDB::addRating(std::string username, int movieId, float rating) {
    //Reading File Object
    std::string dbContent = readFile("{}");

    //Parse the string
    rapidjson::Document d;
    d.Parse(dbContent.c_str());

    //Add movie to users rating list
    auto& allocator = d.GetAllocator();
    if (!d.HasMember(username.c_str())) {                   
        rapidjson::Value userData(rapidjson::kObjectType);  

        rapidjson::Value movie;
        movie.SetFloat(rating);

        std::string s = std::to_string(movieId);
        rapidjson::Value movieKey(s.c_str(), s.size(), allocator);
        userData.AddMember(movieKey, movie, allocator);

        rapidjson::Value usernameKey(username.c_str(), username.size(), allocator);
        d.AddMember(usernameKey, userData, allocator);
    } else {
        rapidjson::Value& userData = d[username.c_str()];

        std::string s = std::to_string(movieId);
        rapidjson::Value movieKey(s.c_str(), s.size(), allocator);

        //Check for duplicates 
        if (userData.HasMember(movieKey)) return false;

        rapidjson::Value movie;
        movie.SetFloat(rating);

        userData.AddMember(movieKey, movie, allocator);
    }

    //Stringify DOM
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    d.Accept(writer);

    //Save File
    writeFile(buffer.GetString());

    return true;
}
//User Delete Function
bool UserDB::deleteUser(std::string username) {

    //Reading the file and converting it to JSON object
    std::string dbContent = readFile("{}");
    rapidjson::Document d;
    d.Parse(dbContent.c_str());

    //Finding if the user exists in the database
    if (d.HasMember(username.c_str()))
    {
        const rapidjson::Value& user = d[username.c_str()];
        d.RemoveMember(user);
        std::cout << "Deleting user: " << username << "\n";
    }
    else
    {
        std::cout<<"User not found: "<<username<<"\n";
    }
    return true;
}
//Reading the films / series voted by the user from the database
std::unordered_map<std::string, float> UserDB::getUserRatings(std::string username) {
    std::unordered_map<std::string, float> result;  // Result Object

    std::string dbContent = readFile("{}");  // Reading the file and converting it to JSON object
    rapidjson::Document d;
    d.Parse(dbContent.c_str());  

    if (d.HasMember(username.c_str())) {                     // Rotate the user's movies if the user exists in the Database
        const rapidjson::Value& user = d[username.c_str()];  
        for (auto& movie : user.GetObject()) {               //Rotate user's movies 
            result[movie.name.GetString()] = movie.value.GetFloat();
        }
    } else {  // Return blank if there is no movie / TV show
        return result;
    }

    return result;
}