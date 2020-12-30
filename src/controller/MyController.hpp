#ifndef MyController_hpp
#define MyController_hpp

#include <string>

#include "algorithm/test.hpp"
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
   public:
    /**
   * Constructor with object mapper.
   * @param objectMapper - default object mapper used to serialize/deserialize DTOs.
   */
    MyController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
        : oatpp::web::server::api::ApiController(objectMapper) {
    }

   public:
    ENDPOINT("GET", "/hello", root) {
        auto dto = MyDto::createShared();
        dto->statusCode = 200;
        dto->message = "Hello World!";
        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT_INFO(test) {
        info->summary = "test";
    }
    ENDPOINT("GET", "test", test) {
        auto dto = TestDTO::createShared();
        dto->statusCode = 200;
        dto->html = "<b>test</b>";
        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT_INFO(jsontest) {
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
    }
};

#include OATPP_CODEGEN_END(ApiController)  //<-- End Codegen

#endif /* MyController_hpp */
