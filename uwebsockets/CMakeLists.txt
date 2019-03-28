# Author: Ivan Blagopoluchnyy 2019 (c)

cmake_minimum_required(VERSION 3.0.2)
project(uwebsockets)

option(INSTALL_HEADERS "Install header files" ON)

find_package(ZLIB REQUIRED)
find_package(OpenSSL REQUIRED)
set(LIBUV_INCLUDE_DIR ${LIBUV_ROOT_DIR}/include)

#set path for the root lib as -DLIBUV_ROOT_DIR
#this solution works if libuv was installed for example by vcpkg (WIN32)
if(WIN32)
	find_library(LIBUV_STATIC_LIBRARY NAMES libuv.lib PATHS ${LIBUV_ROOT_DIR}/lib REQUIRED)
else()
	find_library(LIBUV_LIBRARY NAMES uv libuv REQUIRED)
endif()

if(WIN32)
	list(APPEND LIBUV_LIBRARIES iphlpapi)
	list(APPEND LIBUV_LIBRARIES psapi)
	list(APPEND LIBUV_LIBRARIES userenv)
	list(APPEND LIBUV_LIBRARIES ws2_32)
	list(APPEND LIBUV_LIBRARIES wsock32)
endif()

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(LIBUV DEFAULT_MSG LIBUV_LIBRARIES LIBUV_INCLUDE_DIR)

#create standalone uwebsockets static library
file(GLOB SOURCES src/*.cpp)
add_library(uWS STATIC ${SOURCES})
target_include_directories(uWS PUBLIC ${OPENSSL_INCLUDE_DIR} ${ZLIB_INCLUDE_DIRS})
target_link_libraries(uWS PUBLIC ${OPENSSL_LIBRARIES} ${ZLIB_LIBRARIES} ${LIBUV_STATIC_LIBRARY})
target_compile_features(uWS PRIVATE cxx_range_for)

install(TARGETS uWS
	RUNTIME DESTINATION bin
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
)

if(INSTALL_HEADERS)
	file(GLOB HEADERS src/*.h)
	install(FILES ${HEADERS} DESTINATION include/uWS)
endif()
