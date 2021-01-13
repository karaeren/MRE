#include <algorithm>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <tuple>

#include "similarity.hpp"

class Recomend {
   public:
    std::vector<std::pair<std::string, float>> TopMatches(std::unordered_map<std::string, std::unordered_map<std::string, float>> dataset, std::string person, Similarity* sim);
    std::vector<std::pair<std::string, float>> getRecommendations(std::unordered_map<std::string, std::unordered_map<std::string, float>> dataset, std::string person, Similarity* sim);
    std::unordered_map<std::string, std::vector<std::pair<std::string, float>>> calculateSimilarItem(std::unordered_map<std::string, std::unordered_map<std::string, float>> prefs, Similarity* sim);
    std::vector<std::pair<std::string, float>> getRecommendedItems(std::unordered_map<std::string, std::unordered_map<std::string, float>> prefs, std::unordered_map<std::string, std::unordered_map<std::string, float>> itemSim,std::string user);
};
