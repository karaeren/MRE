
#include <iostream>
#include <tuple>
#include <unordered_map>

#include "recomend.hpp"
#include "similarity.hpp"

using namespace std;

unordered_map<string, float> ranks1;
unordered_map<string, float> ranks2;
unordered_map<string, float> ranks3;
unordered_map<string, float> ranks4;
unordered_map<string, float> ranks5;
unordered_map<string, float> ranks6;
unordered_map<string, float> ranks7;
unordered_map<string, unordered_map<string, float>> critics;

/*
Ustteki template declaration'lari istenilen tipteki container'in bir ornegi olarak tasarlanmistir
Python'daki sozluk(hash) yapisi, ornek datasetin tiplemesi goz onune alinmistir. e.g.

{'Lisa Rose': {'Lady in the Water': 2.5, 'Snakes on a Plane': 3.5,
 'Just My Luck': 3.0, 'Superman Returns': 3.5, 'You, Me and Dupree': 2.5, 
 'The Night Listener': 3.0},
'Gene Seymour': {'Lady in the Water': 3.0, 'Snakes on a Plane': 3.5, 
 'Just My Luck': 1.5, 'Superman Returns': 5.0, 'The Night Listener': 3.0, 
 'You, Me and Dupree': 3.5}, 
'Michael Phillips': {'Lady in the Water': 0.5, 'Snakes on a Plane': 0.7,
 'Superman Returns': 0.65, 'The Night Listener': 0.6, 'Just My Luck': 0.6, 'You, Me and Dupree': 0.58},
'Claudia Puig': {'Snakes on a Plane': 3.5, 'Just My Luck': 3.0,
 'The Night Listener': 4.5, 'Superman Returns': 4.0, 
 'You, Me and Dupree': 2.5},
'Mick LaSalle': {'Lady in the Water': 3.0, 'Snakes on a Plane': 4.0, 
 'Just My Luck': 2.0, 'Superman Returns': 3.0, 'The Night Listener': 3.0,
 'You, Me and Dupree': 2.0}, 
'Jack Matthews': {'Lady in the Water': 3.0, 'Snakes on a Plane': 4.0,
 'The Night Listener': 3.0, 'Superman Returns': 5.0, 'You, Me and Dupree': 3.5},
'Toby': {'Snakes on a Plane':4.5,'You, Me and Dupree':1.0,'Superman Returns':4.0},
'Ali': {'Just My Luck': 1.0,'You, Me and Dupree':2.0,'The Night Listener': 4.5, 'Superman Returns': 1.0, 'Snakes on a Plane':4.5}}


Ilk key degerleri hash tablosundaki kullanici profilini temsil etmektedir.
Ilk value degerleri mevcut profildeki kullanicinin hangi filme ne kadar oy verdigini gosterir.
Puanlama tuple container'i kullanarak tutulmaktadir.
Sonraki eklenecek datasetlerde benzer bir sekilde kullanilacaktir.

*/

int main() {
    ranks1["Lady in the Water"] = 3;
    ranks2["Lady in the Water"] = 3;
    ranks3["Lady in the Water"] = 1;
    ranks4["Lady in the Water"] = 4;
    ranks5["Lady in the Water"] = 3;  //
    ranks6["Lady in the Water"] = 3;
    ranks7["Lady in the Water"] = 6;

    ranks1["Snakes on a Plane"] = 1;
    ranks2["Snakes on a Plane"] = 3;
    ranks3["Snakes on a Plane"] = 4;
    ranks4["Snakes on a Plane"] = 4;
    ranks5["Snakes on a Plane"] = 2;  //
    ranks6["Snakes on a Plane"] = 1;
    ranks7["Snakes on a Plane"] = 5;

    ranks2["Just My Luck"] = 3;
    ranks3["Just My Luck"] = 1;
    ranks4["Just My Luck"] = 2;
    ranks5["Just My Luck"] = 5;
    ranks6["Just My Luck"] = 3;
    ranks7["Just My Luck"] = 5;

    critics["kadir"] = ranks1;
    critics["ayse"] = ranks2;
    critics["fatma"] = ranks3;
    critics["ahmet"] = ranks4;
    critics["mehmet"] = ranks5;
    critics["eren"] = ranks6;
    critics["arafat"]=ranks7;

    for (auto x : critics) {
        cout << endl;
        cout << x.first << " soyle oylar kullandi :" << endl;
        for (auto y : x.second) {
            cout << y.first << " : " << y.second << endl;
        }
    }

    string key = "kadir";
    Recomend a{};
    Similarity* b = nullptr;

    std::cout << "\n";
    std::vector<std::pair<std::string, float>> vec1 = a.TopMatches(critics, key, b);
    for (int i = 0; i < vec1.size(); i++) {
        if (vec1[i].first.empty()) break;
        std::cout << i << " topMatch: " << vec1[i].first << "-" << vec1[i].second << "\n";
    }

    std::cout << "\n";
    std::vector<std::pair<std::string, float>> vec2 = a.getRecommendations(critics, key, b);
    for (int i = 0; i < vec2.size(); i++) {
        std::cout << i << " getRecommendations: " << vec2[i].first << "-" << vec2[i].second << "\n";
    }
    std::unordered_map<std::string, std::unordered_map<std::string, float>> terscritics = b->transformPrefs(critics);
    std::cout << terscritics["Lady in the Water"]["arafat"] << "\n";
    std::cout << terscritics["Lady in the Water"]["eren"] << "\n";
    std::unordered_map<std::string, std::vector<std::pair<std::string, float>>> similaritems = a.calculateSimilarItem(critics, b);
    for (const auto& [key, value] : similaritems) {
        std::cout << key << " is key\n";
        for (auto x : value) {
            if(x.first.empty()) break;
            std::cout << "\tfirst=" << x.first << ", second=" << x.second << "\n";
        }
    }
}
