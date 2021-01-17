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

    // remove empty entries
    int index = 0;
    for (auto i : scores) {
        index++;
        if (!i.first.empty()) continue;
        scores.erase(scores.begin() + index);
    }

    return scores;
}


//Gets recommendations for a person by using a weighted average of every other user's rankings
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
                    if (totals.find(item.first) == totals.end())
                        totals[item.first] = 0.0f;

                    totals[item.first] += dataset[other.first][item.first] * sim_value;
                    len_totals += 1;

                    //Sum of similarities
                    if (simSums.find(item.first) == simSums.end())
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
    rankings.resize(100);  // only 100 recommendations

    return rankings;
}



std::unordered_map<std::string, std::vector<std::pair<std::string, float>>> Recomend::calculateSimilarItem(std::unordered_map<std::string, std::unordered_map<std::string, float>> prefs, Similarity *sim) {
  //  Create a heap of items showing which other items they are most similar to. 
  std::unordered_map<std::string, std::vector<std::pair<std::string, float>>> si;
  
  
  // Invert the preference matrix to be item-centric
    std::unordered_map<std::string, std::unordered_map<std::string, float>> itemPrefs;

    itemPrefs = sim->transformPrefs(prefs);
    for (auto item : itemPrefs) {
        std::vector<std::pair<std::string, float>> scores;
        
        //Find the most similar items to this one
        scores = TopMatches(itemPrefs, item.first, sim);
        si[item.first] = scores;
    }

    return si;
}


std::vector<std::pair<std::string, float>> Recomend::getRecommendedItems(std::unordered_map<std::string, std::unordered_map<std::string, float>> prefs, std::unordered_map<std::string, std::vector<std::pair<std::string, float>>> itemSim, std::string user) {
    std::unordered_map<std::string, float> scores, totalSim, userRatings = prefs[user];

    for (auto item : userRatings) {                     // for each movie the user rated
        for (auto &element : itemSim) {                 // to find iterated move in the itemSim
            if (element.first != item.first) continue;  // continue if it's not the movie we're looking for

            for (auto &item2 : element.second) {                                   // check the similarity score of other movies to this movie
                if (userRatings.find(item2.first) != userRatings.end()) continue;  // skip if user already watched this

                if (scores.find(item2.first) == scores.end())
                    scores[item2.first] = 0.0f;

                scores[item2.first] += item2.second * item.second;

                if (totalSim.find(item2.first) == scores.end())
                    totalSim[item2.first] = 0.0f;

                totalSim[item2.first] += item2.second;
            }

            break;
        }
    }

    std::vector<std::pair<std::string, float>> rankings;

    for (auto i : scores) {
        std::pair<std::string, float> rank;
      //Divide each total score by total weighting to get an average
        rank = make_pair(i.first, i.second / totalSim[i.first]);
        rankings.push_back(rank);
    }
    
    //Return the rankings from highest to lowest
    std::sort(rankings.begin(), rankings.end(), sortByVal);
    return rankings;
}
