cmake_minimum_required(VERSION 3.10.0)
project(SimComp)

add_executable(${PROJECT_NAME} src/SimComp.cpp
src/utils.cpp
src/utils.h)
set(ZLIBPATH "zlib")
target_include_directories(${PROJECT_NAME} PRIVATE ${ZLIBPATH}/include)
target_link_directories(${PROJECT_NAME} PRIVATE ${ZLIBPATH}/lib)
target_link_libraries(${PROJECT_NAME} PRIVATE libz.so)