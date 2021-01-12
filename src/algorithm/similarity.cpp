#include "similarity.hpp"

float Similarity::sim_distance(std::unordered_map<std::string, std::unordered_map<std::string, float> > prefs, std::string person1, std::string person2) {
    std::unordered_map<std::string, float> si;
    std::unordered_map<std::string, float> items_person1;
    std::unordered_map<std::string, float> items_person2;

    items_person1 = prefs[person1];
    items_person2 = prefs[person2];

    for (auto x : items_person1) {
        //x.first = film adi
        //x.second = rank
        for (auto y : items_person2) {
            if (x.first == y.first) {
                si[x.first] = 1;
            }
        }
    }

    int si_counter = 0;
    for (auto i : si) {
        si_counter += 1;
    }
    if (si_counter == 0) {
        return 0.0;
    }

    float sum_of_squares = 0;
    for (auto x : si) {
        std::string item = x.first;
        float farklar = (float)(prefs[person1][item] - prefs[person2][item]);
        sum_of_squares += pow(farklar, 2.0);
    }
    return 1 / (1 + sqrt(sum_of_squares));
}
//
float Similarity::sim_pearson(std::unordered_map<std::string, std::unordered_map<std::string, float> > prefs, std::string person1, std::string person2) {
    std::unordered_map<std::string, float> si;
    std::unordered_map<std::string, float> items_person1;
    std::unordered_map<std::string, float> items_person2;

    items_person1 = prefs[person1];
    items_person2 = prefs[person2];

    for (auto x : items_person1) {
        //x.first = film adi
        //x.second = rank
        for (auto y : items_person2) {
            if (x.first == y.first) {
                si[x.first] = 1;
            }
        }
    }

    int si_counter = 0;
    for (auto i : si) {
        si_counter += 1;
    }
    if (si_counter == 0) {
        return 0.0;
    }

    float sum1 = 0, sum1Sq = 0;
    float sum2 = 0, sum2Sq = 0;
    float pSum = 0;
    for (auto puanlar : si) {
        sum1 += prefs[person1][puanlar.first];
        sum1Sq += pow(prefs[person1][puanlar.first], 2.0);

        sum2 += prefs[person2][puanlar.first];
        sum2Sq += pow(prefs[person2][puanlar.first], 2.0);

        pSum += prefs[person2][puanlar.first] * prefs[person1][puanlar.first];
    }

    //Calculate r (Pearson score)
    float gecici, den;
    gecici = pSum - ((sum1 * sum2) / si_counter);
    den = sqrt((sum1Sq - pow(sum1, 2) / si_counter) * (sum2Sq - pow(sum2, 2) / si_counter));
    if (den == 0) {
        return 0.0f;
    }

    float result;
    result = gecici / den;

    return result;
}