cmake_minimum_required(VERSION 3.15)
project(multithreaded_http_server CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)


add_executable(server
src/main.cpp
src/http.cpp
)


if(UNIX)
target_compile_definitions(server PRIVATE PLATFORM_LINUX)
target_link_libraries(server pthread)
endif()