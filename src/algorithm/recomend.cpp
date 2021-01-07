#include "recomend.hpp"

Recomend::Recomend()
{
}

Recomend::~Recomend()
{
}

tuple < float, string> *Recomend::TopMatches(unordered_map<string, unordered_map<string, int>> dataset, string person, Similarity* sim)
{
    //topMatches fonksiyonu verilen kisi icin diger kisilerle benzerliklerini gosterir.


    
    int n = 5;

    tuple < float, string> scores_array[5];

    int counter = 0;
    for (auto x : dataset) {
        tuple < float, string> score;
        if (x.first != person) {
            //cout << a.sim_distance(critics, person, x.first) << endl;
            score = make_tuple(sim->sim_distance(dataset, person, x.first), x.first);
            //cout << "sa :" << get<0>(score)<<endl;
            scores_array[counter] = score;
        }
        counter += 1;
    }
    //std::cout << endl << person << "'in kullandigi oylarin diger kisilerin kullandigi oylar ile benzerlikleri :" << endl;

    tuple < float, string> new_scores_array[4];//n-1
    int index_counter = 0;
    for (int i = 1; i < 5; i++) {
        //cout<< "benzerlik :" << get<0>(scores_array[i])<<" kisi:"<<get<1>(scores_array[i])<<endl;
        tuple < float, string> gecici;
        for (int q = i + 1; q < 5; q++) {
            if (get<0>(scores_array[i]) > get<0>(scores_array[q])) {
                gecici = scores_array[i];

            }
            else {
                gecici = scores_array[q];
            }
        }
        new_scores_array[index_counter] = gecici;
        index_counter += 1;
    }

    return new_scores_array;
}







tuple < float, string> *Recomend::getRecommendations(unordered_map<string, unordered_map<string, int>> dataset, string person, Similarity* sim){
        unordered_map < string, float > totals, simSums;
    int len_totals = 0;
    for (auto other: dataset) {
        float  sim_value;
        if (other.first != person) {
            sim_value = sim->sim_pearson(dataset, person, other.first);
        }

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
                    totals[item.first] = 0.0f;
                    totals[item.first] += dataset[other.first][item.first] * sim_value;
                    len_totals += 1;
                    //Sum of similarities
                    simSums[item.first] = 0.0f;
                    simSums[item.first] += sim_value;
                }
            }
        }
    }
    int const n = 5;        //

    tuple < float, string> rankings[n];
    int index = 0;
    for (auto i : totals) {//s,f
        tuple < float, string> rank;
        rank = make_tuple(i.second/simSums[i.first], i.first);
        rankings[index] = rank;
    }

    tuple < float, string> new_rankings[n];
    int index_counter = 0;
    for (int i = 1; i < n; i++) {
        //cout<< "benzerlik :" << get<0>(scores_array[i])<<" kisi:"<<get<1>(scores_array[i])<<endl;
        tuple < float, string> gecici;
        for (int q = i + 1; q < n; q++) {
            if (get<0>(rankings[i]) > get<0>(rankings[q])) {
                gecici = rankings[i];

            }
            else {
                gecici = rankings[q];
            }
        }
        new_rankings[index_counter] = gecici;
        index_counter += 1;
    }

    return new_rankings;
}