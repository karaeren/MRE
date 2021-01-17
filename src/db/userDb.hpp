/*
    Header file of database operations
    @author Abdülkadir Furkan Yıldız and Sefa Satıloğlu
*/
#ifndef userdb
#define userdb

#include <string>
#include <unordered_map>

#include "jsonDb.hpp"

class UserDB : public JsonDB {
   public:
   /*
        Functions of database operations  
   */
    UserDB();
    UserDB(std::string location = "") : JsonDB(location) {} //Constructor

    bool addRating(std::string username, int movieId, float rating);
    bool deleteUser(std::string username);

    std::unordered_map<std::string, float> getUserRatings(std::string username);
};

#endif