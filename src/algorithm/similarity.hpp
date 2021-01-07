#ifndef Similarity_HEADER
#define Similarity_HEADER

#include <unordered_map>
#include <math.h>

using namespace std;

class Similarity {
public:
	Similarity();
	~Similarity();

	float sim_distance(unordered_map<string, unordered_map<string, int> > prefs, string person1, string person2);
	float sim_pearson(unordered_map<string, unordered_map<string, int> > prefs, string person1, string person2);
};

#endif