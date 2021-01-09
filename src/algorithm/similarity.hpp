#ifndef Similarity_HEADER
#define Similarity_HEADER

#include <math.h>

#include <unordered_map>

using namespace std;

class Similarity {
   public:
    Similarity();
    ~Similarity();

    float sim_distance(unordered_map<string, unordered_map<string, int> > prefs, string person1, string person2);
    float sim_pearson(unordered_map<string, unordered_map<string, int> > prefs, string person1, string person2);
};

#endif