# Author: Ivan Blagopoluchnyy 2019 (c)

cmake_minimum_required(VERSION 3.0.2)
project(PathMapper)

if(WIN32)
	add_definitions(-DWINVER=0x0600)
	add_definitions(-DUNICODE -D_UNICODE)
endif(WIN32)

if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

add_subdirectory(./lemon)
add_subdirectory(./uwebsockets)

if(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc" )
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread -Wall")
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread -Wall")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

file(GLOB _SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
list(APPEND _LIBS uWS lemon ${LIB_OPENSSL})
set(PROJECT_SOURCE_DEFINITION_DIRECTORY ${PROJECT_SOURCE_DIR}/src)
set(MIDDLEWARE_FILE ${PROJECT_SOURCE_DEFINITION_DIRECTORY}/node_controllers/MiddleWareController.cpp)
set(ENDPOINT_FILE ${PROJECT_SOURCE_DEFINITION_DIRECTORY}/node_controllers/EndPointController.cpp)
file(GLOB _HEADERS SYSTEM "./uwebsockets/src" ${OPENSSL_ROOT_DIR}/include ${PROJECT_SOURCE_DIR}/include)

message(STATUS "lemon root dir: " ${LEMON_SOURCE_DIR})

set(MIDDLEWARE_EXECUTABLE "MiddleWareController")
set(ENDPOINT_EXECUTABLE "EndPointController")
set(BASEPATH "${CMAKE_SOURCE_DIR}")

include_directories(${_HEADERS} ${BASEPATH} ${PROJECT_SOURCE_DIR} ${_LIBS})
add_executable(${MIDDLEWARE_EXECUTABLE} ${MIDDLEWARE_FILE})
add_executable(${ENDPOINT_EXECUTABLE} ${ENDPOINT_FILE})
target_link_libraries("MiddleWareController" ${_LIBS})
target_link_libraries("EndPointController" ${_LIBS})
#cmake -DCMAKE_BUILD_TYPE=release cmake -G "Visual Studio 15 2017 Win64" ..
