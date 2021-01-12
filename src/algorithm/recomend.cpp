#include <iostream>
#include <tuple>
#include <unordered_map>

using namespace std;
#include "recomend.hpp"

Recomend::Recomend() {
}

Recomend::~Recomend() {
}

tuple<float, string>* Recomend::TopMatches(unordered_map<string, unordered_map<string, int>> dataset, string person, Similarity* sim) {
    //topMatches fonksiyonu verilen kisi icin diger kisilerle benzerliklerini gosterir.

    int n = 5;

    static tuple<float, string> scores_array[5];  // n

    int counter = 0;
    for (auto x : dataset) {
        tuple<float, string> score;
        if (x.first != person) {
            //cout << a.sim_distance(critics, person, x.first) << endl;
            score = make_tuple(sim->sim_distance(dataset, person, x.first), x.first);
            cout << x.first << " :" << get<0>(score) << endl;
            scores_array[counter] = score;
        }
        counter += 1;
    }
    //std::cout << endl << person << "'in kullandigi oylarin diger kisilerin kullandigi oylar ile benzerlikleri :" << endl;

    for (int i = 0; i < 5; i++) {
        int gecici_index = i;
        for (int q = i + 1; q < 5; q++) {
            if (get<0>(scores_array[q]) > get<0>(scores_array[gecici_index])) {
                gecici_index = q;
            }
        }
        if (i != gecici_index)
            swap(scores_array[i], scores_array[gecici_index]);
    }

    return scores_array;
}

tuple<float, string>* Recomend::getRecommendations(unordered_map<string, unordered_map<string, int>> dataset, string person, Similarity* sim) {
    unordered_map<string, float> totals, simSums;
    int len_totals = 0;
    for (auto other : dataset) {
        if (other.first == person) continue;

        float sim_value = sim->sim_pearson(dataset, person, other.first);
        std::cout << other.first << " sim_value:" << sim_value << "\n";
        if (sim_value > 0) {
            for (auto item : dataset[other.first]) {
                bool is_item_in_dataset_person = false;
                for (auto i : dataset[person]) {
                    if (i.first == item.first) {
                        is_item_in_dataset_person = true;
                    }
                }

                if (is_item_in_dataset_person == false || dataset[person][item.first] == 0) {
                    //Similarity * Score
                    totals[item.first] = 0.0f;//burası kanka
                    totals[item.first] += dataset[other.first][item.first] * sim_value;
                    len_totals += 1;
                    //Sum of similarities
                    simSums[item.first] = 0.0f;
                    simSums[item.first] += sim_value;
                }
            }
        }
    }
    int const n = 5;  //

    static tuple<float, string> rankings[n];
    int index = 0;
    for (auto i : totals) {
        tuple<float, string> rank;
        rank = make_tuple(i.second / simSums[i.first], i.first);
        rankings[index] = rank;
        index++;
    }

    for (int i = 0; i < 5; i++) {
        int gecici_index = i;
        for (int q = i + 1; q < 5; q++) {
            if (get<0>(rankings[q]) > get<0>(rankings[gecici_index])) {
                gecici_index = q;
            }
        }
        if (i != gecici_index)
            swap(rankings[i], rankings[gecici_index]);
    }

    return rankings;
}
unordered_map<string, unordered_map<string, int> > Recomend::calculateSimilarItem(unordered_map<string, unordered_map<string, int> > prefs,Similarity* sim) {
    unordered_map<string,tuple<float, string>*> si;
    unordered_map<string, unordered_map<string, int>> itemPrefs;
    
    int bayrak=0;
    int n=10;
    itemPrefs=sim->transformPrefs(prefs);
    for(auto item:itemPrefs){
        bayrak+=1;
        if (bayrak%100==0){
            //ekrana yazdırmamıza gerek yok

        }
        tuple<float, string>* scores;
        scores=TopMatches(itemPrefs,item.first,sim);
        si[item.first]=scores;
        
    }
    return si;
}