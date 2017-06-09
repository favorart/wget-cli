cmake_minimum_required(VERSION 3.7)
project(wget_client)

set(CMAKE_CXX_STANDARD 11)
set(DCMAKE_CXX_FLAGS -std=c++11)

set(SOURCE_FILES wget/header.h        wget/crawler.cpp   wget/crawler.h   wget/main.cpp
                 wget/parse_html.cpp  wget/con_args.cpp  wget/con_args.h  wget/urls.cpp wget/sockets.cpp)
add_executable(wget_client ${SOURCE_FILES})

find_package ( cppnetlib 0.11.0 REQUIRED )
include_directories ( ${CPPNETLIB_INCLUDE_DIRS} )
target_link_libraries ( wget_client ${CPPNETLIB_LIBRARIES} )

find_library(Gumbo_LIBRARY
             NAMES libgumbo.dylib libgumbo.dll gumbo.dylib gumbo.dll libgumbo.so)
target_link_libraries ( wget_client ${Gumbo_LIBRARY} )


find_package(CURL)
IF(CURL_FOUND)
    message(STATUS "Curl libraries found at: ${CURL_LIBRARIES}")
    message(STATUS "Curl includes found at: ${CURL_INCLUDE_DIRS}")
else()
    message(SEND_ERROR "Could not find cURL on your system")
ENDIF(CURL_FOUND)

target_link_libraries(wget_client ${CURL_LIBRARIES})
