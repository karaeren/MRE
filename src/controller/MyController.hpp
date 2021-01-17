/**
    Controller file. Endpoints and their purpouses and also their functions are decided here.
    @file MyController.HPP
*/

#ifndef MyController_hpp
#define MyController_hpp

#include <assert.h>

#include <string>
#include <unordered_map>

#include "algorithm/recomend.hpp"
#include "algorithm/similarity.hpp"
#include "db/jsonDb.hpp"
#include "db/userDb.hpp"
#include "dto/DTOs.hpp"
/* frameworks */
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/web/server/api/ApiController.hpp"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

#include OATPP_CODEGEN_BEGIN(ApiController)  //<-- Begin Codegen

class MyController : public oatpp::web::server::api::ApiController {
   private:
    std::unordered_map<std::string, std::unordered_map<std::string, float>> critics;  // Algorithms use this file to parse every rating there is.
    // Instances for classes that are used by algorithms and endpoints
    Similarity* sim_class;
    Recomend reco_class;
    UserDB* udb_instance;

   public:
    /**
   * Constructor with object mapper.
   * @param objectMapper - default object mapper used to serialize/deserialize DTOs.
   */
    MyController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
        : oatpp::web::server::api::ApiController(objectMapper) {
        // initialize instances with empty new classes
        sim_class = nullptr;
        reco_class = {};
        udb_instance = new UserDB("../data/user.json");

        std::cout << "Starting to parse the dataset...\n";

        // This is where the dataset for movie ratings are held
        // because there is no __dirname and such, it is important
        // to run the program from the "dist" directory.
        JsonDB db("../data/ratings.json");
        const std::string dbContent = db.readFile();

        rapidjson::Document d;
        d.Parse(dbContent.c_str());
        assert(d.IsObject());

        const rapidjson::Value& r = d["ratings"];
        assert(r.IsArray());

        for (rapidjson::SizeType i = 0; i < r.Size(); i++) {
            const rapidjson::Value& rating = r[i];
            assert(rating.IsObject());

            if (critics.find(std::to_string(rating["uid"].GetInt())) == critics.end()) {
                std::unordered_map<std::string, float> temp;
                critics[std::to_string(rating["uid"].GetInt())] = temp;
            }

            auto ele = critics.find(std::to_string(rating["uid"].GetInt()));
            ele->second[std::to_string(rating["id"].GetInt())] = rating["r"].GetFloat();
        }

        std::cout << "Done with the dataset parsing.\n";
    }

   public:
    /*
    Root endpoint. Currently used for nothing so we just display server status...
   */
    ENDPOINT("GET", "/", root) {
        auto dto = RootDTO::createShared();

        dto->statusCode = 200;
        dto->result = "Server working properly!";

        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT_INFO(recommendedMovies) {
        info->summary = "List recommended movies for the user based on their preferences.";
    }
    ADD_CORS(recommendedMovies);
    ENDPOINT("GET", "recommended-movies", recommendedMovies,
             QUERY(String, username, "u"),
             QUERY(String, type, "t")) {
        auto dto = RecommendedMoviesDTO::createShared();

        // get user from db
        std::unordered_map<std::string, float> user_ratings = udb_instance->getUserRatings(username->std_str());

        // check if user is persistent in the critics map
        // create a new entry if not
        if (critics.find(username->std_str()) == critics.end()) {
            std::unordered_map<std::string, float> temp;
            critics[username->std_str()] = temp;
        }

        // for each user rating, add/update it on the critics map
        for (auto x : user_ratings) {
            auto ele = critics.find(username->std_str());
            ele->second[x.first] = x.second;
        }

        std::string type_str = type->std_str(),
                    username_str = username->std_str();
        std::vector<std::pair<std::string, float>> recommendedMovies;

        // Get recommendations based on username ("u") and type ("t").
        // Types are "user-match" and "item-match"
        if (type_str.compare("user-match") == 0) {
            recommendedMovies = reco_class.getRecommendations(critics, username_str, sim_class);
        } else if (type_str.compare("item-match") == 0) {
            std::unordered_map<std::string, std::vector<std::pair<std::string, float>>> similaritems = reco_class.calculateSimilarItem(critics, sim_class);
            recommendedMovies = reco_class.getRecommendedItems(critics, similaritems, username_str);
        } else {
            dto->statusCode = 400;
            dto->result = "{\"error\":\"MATCH_TYPE_NOT_FOUND\"}";
            return createDtoResponse(Status::CODE_400, dto);
        }

        if (recommendedMovies.size() == 0) {
            dto->statusCode = 406;
            dto->result = "{\"error\":\"NO_RECOMMENDATION\"}";
            return createDtoResponse(Status::CODE_406, dto);
        }

        // create a new Document object for sending the results
        rapidjson::Document d;
        d.SetObject();
        rapidjson::Document::AllocatorType& allocator = d.GetAllocator();

        rapidjson::Value movies(rapidjson::kObjectType);
        rapidjson::Value val(rapidjson::kObjectType);

        // add each recommended movie to the temporary value
        for (int i = 0; i < recommendedMovies.size(); i++) {
            if (recommendedMovies[i].first.empty()) break;

            val.SetFloat(recommendedMovies[i].second);
            rapidjson::Value movieKey(recommendedMovies[i].first.c_str(), recommendedMovies[i].first.size(), allocator);
            movies.AddMember(movieKey, val, allocator);
        }

        // add the temporary value to the document
        d.AddMember("movies", movies, allocator);

        // stringify document
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        d.Accept(writer);

        // send the stringified JSON document
        dto->result = buffer.GetString();
        dto->statusCode = 200;

        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT_INFO(rateMovie) {
        info->summary = "Let the user rate a movie from 1 to 5 (0.5 increments).";
    }
    ADD_CORS(rateMovie);
    ENDPOINT("GET", "/rate-movie", rateMovie,
             QUERY(String, username, "u"),
             QUERY(Int32, movieId, "m"),
             QUERY(Float32, rating, "r")) {
        auto dto = RateMovieDTO::createShared();
        // add new rating to the db under the name of the user and see if it's ok
        bool rating_added = udb_instance->addRating(username->std_str(), movieId, rating);

        if (rating_added) {
            dto->statusCode = 200;
            dto->result = "{\"error\":\"\"}";
            return createDtoResponse(Status::CODE_200, dto);
        } else {
            dto->statusCode = 400;
            dto->result = "{\"error\":\"RATING_ALREADY_EXISTS\"}";
            return createDtoResponse(Status::CODE_400, dto);
        }
    }
};

#include OATPP_CODEGEN_END(ApiController)  //<-- End Codegen

#endif /* MyController_hpp */
