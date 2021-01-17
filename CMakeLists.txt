cmake_minimum_required(VERSION 3.1)

set(project_name mre)

project(${project_name})

include_directories(include)

set(CMAKE_CXX_STANDARD 11)

add_library(${project_name}-lib
        src/AppComponent.hpp
        src/controller/MyController.cpp
        src/controller/MyController.hpp
        src/dto/DTOs.hpp
        src/algorithm/similarity.cpp
        src/algorithm/similarity.hpp
        src/algorithm/recomend.cpp
        src/algorithm/recomend.hpp
        src/db/jsonDb.hpp
        src/db/userDb.cpp
        src/db/userDb.hpp
)

## link libs

find_package(oatpp 1.2.5 REQUIRED)

target_link_libraries(${project_name}-lib
        PUBLIC oatpp::oatpp
        PUBLIC oatpp::oatpp-test
)

target_include_directories(${project_name}-lib PUBLIC src)

## add executables

add_executable(${project_name}-exe
        src/App.cpp)
target_link_libraries(${project_name}-exe ${project_name}-lib)
add_dependencies(${project_name}-exe ${project_name}-lib)
