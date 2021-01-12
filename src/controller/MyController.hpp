#ifndef MyController_hpp
#define MyController_hpp

#include <assert.h>

#include <string>
#include <unordered_map>

#include "db/jsonDb.hpp"
#include "dto/DTOs.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/web/server/api/ApiController.hpp"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

#include OATPP_CODEGEN_BEGIN(ApiController)  //<-- Begin Codegen

class MyController : public oatpp::web::server::api::ApiController {
   private:
    std::unordered_map<int, std::unordered_map<int, float> > critics;

   public:
    /**
   * Constructor with object mapper.
   * @param objectMapper - default object mapper used to serialize/deserialize DTOs.
   */
    MyController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
        : oatpp::web::server::api::ApiController(objectMapper) {
        std::cout << "Starting to parse the dataset...\n";

        JsonDB db("../data/ratings.json");
        const std::string dbContent = db.readFile();

        // 1. Parse JSON string into DOM.
        rapidjson::Document d;
        d.Parse(dbContent.c_str());
        assert(d.IsObject());

        const rapidjson::Value& r = d["ratings"];
        assert(r.IsArray());

        for (rapidjson::SizeType i = 0; i < r.Size(); i++) {
            const rapidjson::Value& rating = r[i];
            assert(rating.IsObject());

            if (critics.find(rating["uid"].GetInt()) == critics.end()) {
                std::unordered_map<int, float> temp;
                critics[rating["uid"].GetInt()] = temp;
            }

            auto ele = critics.find(rating["uid"].GetInt());
            ele->second[rating["id"].GetInt()] = rating["r"].GetFloat();
        }

        std::cout << "Done with the dataset parsing.\n";
    }

   public:
    ENDPOINT("GET", "/", root) {
        auto dto = RootDTO::createShared();

        // Debugging stuff. TODO: Remove this :)
        for(auto x : critics) {
            std::cout << "Critic " << x.first << " voted these movies:\n";
            for(auto y : x.second) {
                std::cout << "Movie: " << y.first << ", rating: " << y.second << "\n";
            }
        }

        dto->statusCode = 200;
        dto->result = "Server working properly!";

        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT_INFO(recommendedMovies) {
        info->summary = "List recommended movies for the user based on their preferences.";
    }
    ENDPOINT("GET", "recommended-movies", recommendedMovies,
             QUERY(String, username, "u"),
             QUERY(String, type, "t")) {
        auto dto = RecommendedMoviesDTO::createShared();

        dto->statusCode = 200;
        // TODO: Get recommendations based on username ("u") and type ("t").
        // Types are "userMatch" and "itemMatch"
        dto->result = "movie list";

        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT_INFO(rateMovie) {
        info->summary = "Let the user rate a movie from 1 to 5 (0.5 increments).";
    }
    ENDPOINT("GET", "/rate-movie", rateMovie,
             QUERY(String, username, "u"),
             QUERY(Int32, movieId, "m"),
             QUERY(Float32, rating, "r")) {

        auto dto = RateMovieDTO::createShared();

        dto->statusCode = 200;
        dto->result = "Success";

        return createDtoResponse(Status::CODE_200, dto);
    }
};

#include OATPP_CODEGEN_END(ApiController)  //<-- End Codegen

#endif /* MyController_hpp */
