#include "userDb.hpp"

#include <string>

#include "oatpp/core/Types.hpp"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

bool UserDB::addRating(std::string username, int movieId, float rating) {
    // write db contents to a string
    std::string dbContent = readFile("{\"users\":{}");

    // parse the string
    rapidjson::Document d;
    d.Parse(dbContent.c_str());

    // add movie to users rating list
    auto& allocator = d.GetAllocator();
    if (!d.HasMember(username.c_str())) { // first entry of user
        rapidjson::Value userData(rapidjson::kObjectType);  // empty object

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
        
        // check for duplicates
        if (userData.HasMember(movieKey)) return false;

        rapidjson::Value movie;
        movie.SetFloat(rating);

        userData.AddMember(movieKey, movie, allocator);
    }

    // stringify dom
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    d.Accept(writer);

    // save file
    writeFile(buffer.GetString());

    return true;
}

bool UserDB::deleteUser(std::string username) {
    std::cout << "Deleting user: " << username << "\n";
    // todo: databaseden sil

    return true;
}

std::unordered_map<std::string, int> UserDB::getUserRatings(std::string username) {
    static std::unordered_map<std::string, int> result;

    // add users movies into static unordered map and return it.
    // example way to add it.
    result["Lord of The Rings"] = 5.0;
    result["Hobbit"] = 4.0;

    return result;
}