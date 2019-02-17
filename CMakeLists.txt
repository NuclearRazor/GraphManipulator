# Author: Ivan Blagopoluchnyy 2018 (c)

cmake_minimum_required( VERSION 3.0.2 )

add_subdirectory(./uwebsockets)

if(MSVC)
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  /EHsc" )
	#set( CMAKE_VS_PLATFORM_TOOLSET "LLVM-vs2017") #if need to use llvm compiler
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")
endif()

project(PathMapper )

set (PROJECT_SOURCE_DECLARATION_DIRECTORY ${PROJECT_SOURCE_DIR}/include)
set (PROJECT_SOURCE_DEFINITION_DIRECTORY ${PROJECT_SOURCE_DIR}/src)
set (MAIN_FILE ${PROJECT_SOURCE_DEFINITION_DIRECTORY}/PathMapper.cpp)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(boost_USE_STATIC_LIBS ON)

find_package(Boost COMPONENTS REQUIRED system filesystem graph)

set(BOOST_USE_MULTITHREADED ON)
set(BOOST_USE_STATIC_RUNTIME OFF)
set(BOOST_ALL_DYN_LINK OFF)

if(Boost_FOUND)
	include_directories (SYSTEM
					 ${PROJECT_SOURCE_DECLARATION_DIRECTORY} 
					 ${Boost_INCLUDE_DIR}					 
					 "${OPENSSL_ROOT_DIR}/include")
else()
	message(FATAL_ERROR "CANNOT FIND BOOST LIBS")
endif ()

add_executable (PathMapper ${MAIN_FILE})

target_link_libraries (PathMapper
					   uWS
			           ${Boost_LIBRARIES} 
					   ${LIB_OPENSSL} )

					   