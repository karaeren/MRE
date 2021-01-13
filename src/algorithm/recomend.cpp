#include "recomend.hpp"

bool sortByVal(const std::pair<std::string, float> &a,
               const std::pair<std::string, float> &b) {
    return (a.second > b.second);
}

std::vector<std::pair<std::string, float>> Recomend::TopMatches(std::unordered_map<std::string, std::unordered_map<std::string, float>> dataset, std::string person, Similarity *sim) {
    //topMatches fonksiyonu verilen kisi icin diger kisilerle benzerliklerini gosterir.

    std::vector<std::pair<std::string, float>> scores;

    for (auto other : dataset) {
        if (other.first == person) continue;

        std::pair<std::string, float> score;
        score = make_pair(other.first, sim->sim_distance(dataset, person, other.first));
        scores.push_back(score);
    }

    scores.resize(20);  // only keep first 20 matches
    return scores;
}

std::vector<std::pair<std::string, float>> Recomend::getRecommendations(std::unordered_map<std::string, std::unordered_map<std::string, float>> dataset, std::string person, Similarity *sim) {
    std::unordered_map<std::string, float> totals, simSums;

    int len_totals = 0;
    for (auto other : dataset) {
        if (other.first == person) continue;

        float sim_value = sim->sim_pearson(dataset, person, other.first);

        if (sim_value > 0) {
            for (auto item : dataset[other.first]) {
                bool person_already_watched_movie = false;
                for (auto i : dataset[person]) {
                    if (i.first == item.first) {
                        person_already_watched_movie = true;
                    }
                }

                if (person_already_watched_movie == false || dataset[person][item.first] == 0) {
                    //Similarity * Score
                    totals[item.first] = 0.0f;  //burası kanka
                    totals[item.first] += dataset[other.first][item.first] * sim_value;
                    len_totals += 1;

                    //Sum of similarities
                    simSums[item.first] = 0.0f;
                    simSums[item.first] += sim_value;
                }
            }
        }
    }

    std::vector<std::pair<std::string, float>> rankings;

    for (auto i : totals) {
        std::pair<std::string, float> rank;
        rank = make_pair(i.first, i.second / simSums[i.first]);
        rankings.push_back(rank);
    }

    std::sort(rankings.begin(), rankings.end(), sortByVal);

    return rankings;
}

std::unordered_map<std::string, std::vector<std::pair<std::string, float>>> Recomend::calculateSimilarItem(std::unordered_map<std::string, std::unordered_map<std::string, float>> prefs, Similarity *sim) {
    std::unordered_map<std::string, std::vector<std::pair<std::string, float>>> si;
    std::unordered_map<std::string, std::unordered_map<std::string, float>> itemPrefs;

    int bayrak = 0;
    int n = 10;
    itemPrefs = sim->transformPrefs(prefs);
    for (auto item : itemPrefs) {
        bayrak += 1;
        if (bayrak % 100 == 0) {
            //ekrana yazdırmamıza gerek yok
        }
        std::vector<std::pair<std::string, float>> scores;
        scores = TopMatches(itemPrefs, item.first, sim);
        si[item.first] = scores;
    }

    return si;
}
std::vector<std::pair<std::string, float>> Recomend::getRecommendedItems(std::unordered_map<std::string, std::unordered_map<std::string, float>> prefs,  std::unordered_map<std::string, std::unordered_map<std::string, float>> itemSim,std::string user) {
    std::unordered_map<std::string, float> scores, totalSim,userRatings=prefs[user];
    for(auto item:userRatings){
        for(auto item2: itemSim[item.first]){
            for(auto rati:userRatings){
                if (item2 != rati){
                    scores={};
                    scores[item2.first]+=item2.second*item.second;
                    totalSim={};
                    totalSim[item2.first]+=item2.second;
                }
            }
        }
    }
    std::vector<std::pair<std::string, float>> rankings;

    for (auto i : scores) {
        std::pair<std::string, float> rank;
        rank = make_pair(i.first, i.second / totalSim[i.first]);
        rankings.push_back(rank);
    }

    std::sort(rankings.begin(), rankings.end(), sortByVal);

    return rankings;

}