#include "userDb.hpp"

#include <string>

#include "oatpp/core/Types.hpp"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

bool UserDB::addRating(std::string username, int movieId, float rating) {
    // write db contents to a string
    std::string dbContent = readFile("{}");

    // parse the string
    rapidjson::Document d;
    d.Parse(dbContent.c_str());

    // add movie to users rating list
    auto& allocator = d.GetAllocator();
    if (!d.HasMember(username.c_str())) {                   // first entry of user
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

std::unordered_map<std::string, float> UserDB::getUserRatings(std::string username) {
    std::unordered_map<std::string, float> result;  // sonuç objesi

    std::string dbContent = readFile("{}");  // dosyayı oku
    rapidjson::Document d;
    d.Parse(dbContent.c_str());  // dosyayı json objesine dönüştür

    if (d.HasMember(username.c_str())) {                     // eğer kullanıcı databasede varsa
        const rapidjson::Value& user = d[username.c_str()];  // kullanıcı objesi
        for (auto& movie : user.GetObject()) {               // kullanıcının filmlerini döndür
            result[movie.name.GetString()] = movie.value.GetFloat();
        }
    } else {  // yoksa boş döndür
        return result;
    }

    return result;
}