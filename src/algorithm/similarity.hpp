#ifndef Similarity_HEADER
#define Similarity_HEADER

#include <math.h>

#include <string>
#include <unordered_map>

class Similarity {
   public:
    float sim_distance(std::unordered_map<std::string, std::unordered_map<std::string, float> > prefs, std::string person1, std::string person2);
    float sim_pearson(std::unordered_map<std::string, std::unordered_map<std::string, float> > prefs, std::string person1, std::string person2);
    std::unordered_map<std::string, std::unordered_map<std::string, float> > transformPrefs(std::unordered_map<std::string, std::unordered_map<std::string, float> > prefs);
};

#endif