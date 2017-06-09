//
// Created by favorart on 30.05.17.
//

#ifndef WGET_CLIENT_HEADER_H
#define WGET_CLIENT_HEADER_H
// ------------------------------------
// #include <cstdarg>
#include <string>
#include <locale>   // std::locale, std::tolower
#include <vector>
#include <fstream>
#include <iostream>
#include <typeinfo>
#include <exception>
#include <unordered_set>

#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <regex>
// ------------------------------------
extern "C"
{
#include <syslog.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <unistd.h>
}
// ------------------------------------
const char kPathSeparator =
#ifdef _WIN32
        '\\';
#else
        '/';
#endif
// ------------------------------------
#define IN
#define OUT
// ------------------------------------
typedef std::unordered_set<std::string> hashTableURLs;
void         url_prepare (IN OUT   std::string &url);
std::string  url_host    (IN const std::string &url);
std::string  url_path    (IN const std::string &url);
// ------------------------------------
std::string escape_string (IN const std::string &str);
// ------------------------------------
void  lower_case (IN OUT std::string &str);
// ------------------------------------
const int MAX_DATA_SIZE = 1000; // max number of bytes we can get at once
// ================================================================
bool socket_connect_send_and_receive (IN const std::string &url, IN int port_no,
                                      IN const std::string &request,
                                      OUT      std::string &responce, IN int size=MAX_DATA_SIZE,
                                      IN bool  verbose=false);
// ------------------------------------
#endif //WGET_CLIENT_HEADER_H
