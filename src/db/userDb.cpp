#include "userDb.hpp"

#include <string>

#include "oatpp/core/Types.hpp"

bool UserDB::addRating(std::string username, int movieId, float rating) {
    std::cout << "Saving: " << username << " voted " << movieId << " with a score of " << rating << "\n";
    // todo: database kaydet

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