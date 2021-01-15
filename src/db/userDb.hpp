#ifndef userdb
#define userdb

#include <string>
#include <unordered_map>

#include "jsonDb.hpp"

class UserDB : public JsonDB {
   public:
    UserDB();
    UserDB(std::string location = "") : JsonDB(location) {}

    bool addRating(std::string username, int movieId, float rating);
    bool deleteUser(std::string username);

    std::unordered_map<std::string, float> getUserRatings(std::string username);
};

#endif