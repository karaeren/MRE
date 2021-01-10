#include <tuple>
#include <unordered_map>

#include "similarity.hpp"

using namespace std;

class Recomend {
   public:
    Recomend();
    ~Recomend();

    tuple<float, string>* TopMatches(unordered_map<string, unordered_map<string, int> > dataset, string person, Similarity* sim);
    tuple<float, string>* getRecommendations(unordered_map<string, unordered_map<string, int> > dataset, string person, Similarity* sim);
};
