#ifndef MyController_hpp
#define MyController_hpp

#include <string>
#include <unordered_map>

#include "algorithm/test.hpp"
#include "db/jsonDb.hpp"
#include "db/userDb.hpp"
#include "dto/DTOs.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/web/server/api/ApiController.hpp"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

#include OATPP_CODEGEN_BEGIN(ApiController)  //<-- Begin Codegen

class MyController : public oatpp::web::server::api::ApiController {
   public:
    /**
   * Constructor with object mapper.
   * @param objectMapper - default object mapper used to serialize/deserialize DTOs.
   */
    MyController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
        : oatpp::web::server::api::ApiController(objectMapper) {
    }

   public:
    ENDPOINT("GET", "/", root) {
        auto dto = RootDTO::createShared();

        dto->statusCode = 200;
        dto->message = "Hello World!";

        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT_INFO(movies) {
        info->summary = "List movies in the database.";
    }
    ENDPOINT("GET", "movies", movies) {
        auto dto = MoviesDTO::createShared();

        dto->statusCode = 200;

        // TODO: List movies to the user with pagination system.
        dto->page = 1;
        dto->movies = "movie list";

        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT_INFO(recommendedMovies) {
        info->summary = "List recommended movies for the user based on their preferences.";
    }
    ENDPOINT("GET", "recommended-movies", recommendedMovies,
             QUERY(String, username, "u")) {
        auto dto = RecommendedMoviesDTO::createShared();

        dto->statusCode = 200;

        // TODO: List movies to the user with pagination system.
        dto->movies = "movie list";

        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT_INFO(rateMovie) {
        info->summary = "Let the user rate a movie from 0 to 5.";
    }
    ENDPOINT("GET", "/rate-movie", rateMovie,
             QUERY(String, username, "u"),
             QUERY(Int32, movieId, "m"),
             QUERY(Float32, rating, "r")) {
        //std::cout << username->getData() << " voted " << movieId << " with a score of " << rating << "\n";

        auto dto = RateMovieDTO::createShared();

        UserDB udb_instance("../data/user.json");
        bool test = udb_instance.addRating(username->std_str(), movieId, rating);

        std::cout << "Kullanicinin filmlerini getUserRatings'den aliyoruz:\n";
        std::unordered_map<std::string, int> test_map = udb_instance.getUserRatings(username->std_str());
        for (auto x : test_map) {
            std::cout << username->std_str() << " voted " << x.first << " with a score of " << x.second << "\n";
        }
        
        dto->statusCode = 200;
        dto->result = "success";

        return createDtoResponse(Status::CODE_200, dto);
    }

    /* ENDPOINT_INFO(jsontest) {
        info->summary = "test json stuff";
    }
    ENDPOINT("GET", "testjson", jsontest) {
        JsonDB db("../data/test.json");
        const std::string dbContent = db.readFile("{\"project\":\"MRE\",\"dbType\":\"json\",\"count\": 0}");

        // 1. Parse a JSON string into DOM.
        rapidjson::Document d;
        d.Parse(dbContent.c_str());

        // 2. Modify it by DOM.
        rapidjson::Value& dbType = d["dbType"];
        dbType.SetString("JSON");

        rapidjson::Value& count = d["count"];
        count.SetInt(count.GetInt() + 1);

        // 3. Stringify the DOM
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        d.Accept(writer);

        // Output
        db.writeFile(buffer.GetString());

        auto dto = TestDTO::createShared();
        dto->statusCode = 200;
        dto->html = "done";
        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT_INFO(simpleMathRoute) {
        // general
        info->summary = "Add x to y";
        info->addResponse<Object<MathDTO>>(Status::CODE_200, "application/json");
        // params specific
        info->pathParams["x"].description = "x";
        info->pathParams["y"].description = "y";
    }
    ENDPOINT("GET", "/simpleMath/{x}/{y}", simpleMathRoute,
             PATH(Int32, x),
             PATH(Int32, y)) {
        int res = test::simpleMath(x, y);

        auto dto = MathDTO::createShared();
        dto->result = res;

        return createDtoResponse(Status::CODE_200, dto);
    } */
};

#include OATPP_CODEGEN_END(ApiController)  //<-- End Codegen

#endif /* MyController_hpp */
