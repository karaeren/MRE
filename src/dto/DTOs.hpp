#ifndef DTOs_hpp
#define DTOs_hpp

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include "rapidjson/document.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 *  Data Transfer Object. Object containing fields only.
 *  Used in API for serialization/deserialization and validation
 */
class RootDTO : public oatpp::DTO {
    DTO_INIT(RootDTO, DTO)

    DTO_FIELD(Int32, statusCode);

    DTO_FIELD(String, message);
};

class MoviesDTO : public oatpp::DTO {
    DTO_INIT(MoviesDTO, DTO)

    DTO_FIELD(Int32, statusCode);

    DTO_FIELD(Int32, page);
    DTO_FIELD(String, movies);
};

class RecommendedMoviesDTO : public oatpp::DTO {
    DTO_INIT(RecommendedMoviesDTO, DTO)

    DTO_FIELD(Int32, statusCode);

    DTO_FIELD(String, movies);
};

class RateMovieDTO : public oatpp::DTO {
    DTO_INIT(RateMovieDTO, DTO)

    DTO_FIELD(Int32, statusCode);

    DTO_FIELD(String, result);
};

#include OATPP_CODEGEN_END(DTO)

#endif /* DTOs_hpp */
