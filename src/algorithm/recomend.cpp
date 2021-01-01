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
