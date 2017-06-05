#pragma once
// Created by favorart on 02.06.17.
//

#ifndef WGET_CLIENT_CRAWLER_H
#define WGET_CLIENT_CRAWLER_H

#include "../wget/header.h"

// #include <cstdarg>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <typeinfo>
#include <exception>
#include <unordered_set>

#include <cstdlib>
#include <cstdio>
#include <ctime>

#include <boost/regex.hpp>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>
// #include <boost/algorithm.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/network/uri.hpp>
#include <boost/network/uri/uri_io.hpp>
#include <boost/asio.hpp>

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



const int MAXRECV = (140 * 1024);
bool  socket_connect_and_receive (IN const char *host, IN int port,
                                  IN const std::string *request,
                                  OUT std::string *responce);
}


class wgetException : std::exception
{
    virtual const char *what () const throw ()
    { return "wget-cli download error"; }
};
class wgetConnectErrorException : std::exception
{};
class wgetNotSupportException   : std::exception
{};


inline std::string   escape_string (const std::string &str)
{
 const boost::regex  escape ("[.^$|()\\[\\]{}*+?\\\\]");
 const std::string   replace_to ("\\\\&");

 return  boost::regex_replace (str, escape, replace_to,
                               boost::match_default | boost::format_sed);
}


typedef std::unordered_set<std::string> hashtableURIs;


class wget
{
    boost::filesystem::path saveDir;
    
    int  delay;
    int  tries;
    bool recursive;
    bool noParent;
    bool pageRequist;
    
    hashtableURIs crawled;

public:
    wget (IN const std::string &saveDir,
          IN int  tries = 1,
          IN bool recursive = false,
          IN bool noParent = false,
          IN bool pageRequist = false,
          IN int delay = 12) :
            saveDir (saveDir), tries (tries), noParent (noParent),
            pageRequist (pageRequist), delay (delay)
    {
        openlog ("wget_cli_log", LOG_PID | LOG_CONS, LOG_USER);
        syslog (LOG_INFO, "BEGIN wget");
        
        struct stat statsStruct = {};
        if ( stat (saveDir.c_str (), &statsStruct) != 0
             && (statsStruct.st_mode & S_IFDIR) == 0)
        {
            syslog (LOG_ERR, "ERROR save folder does not exist");
            throw wgetException ();
        }
        
        if ( 0 > tries || tries > 100 )
        {
            syslog (LOG_ERR, "ERROR incorrect number of tries");
            throw wgetException ();
        }
    
        if ( 0 > delay || delay > 10000 )
        {
            syslog (LOG_ERR, "ERROR invalid delay");
            throw wgetException ();
        }
    }
    
    virtual ~wget ()
    {
        syslog (LOG_INFO, "END wget");
        closelog ();
    }
    
    void download (IN const boost::network::uri::uri &url,
                   IN       int   port=80,
                   IN const int   depth=1,
                   IN const bool  continueDump=false,
                   IN const bool  verbose=false);
        
protected:
    bool         splitHeader     (IN OUT std::string &responce, IN bool verbose);
    std::string  formGetRequest  (IN const boost::network::uri::uri &url);
    std::string  formOutFileName (IN const boost::network::uri::uri &url);
    void         dumpOutFile     (IN const std::string &filename,
                                  IN const std::string &responce);
    void         parseHTML       (IN const std::string &html,
                                  OUT hashtableURIs &urls);

//    bool fetch (IN  const boost::network::uri::uri &url,
//                IN        int          port,
//                IN  const std::string &request,
//                OUT       std::string &responce);
//
//    void  connect  (IN const boost::network::uri::uri &url);
};

#endif // WGET_CLIENT_CRAWLER_H
