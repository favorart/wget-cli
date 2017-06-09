#pragma once
// Created by favorart on 02.06.17.
//

#ifndef WGET_CLIENT_CRAWLER_H
#define WGET_CLIENT_CRAWLER_H

#include "header.h"


class wgetException : std::exception
{
    virtual const char *what () const throw ()
    { return "wget-cli download error"; }
};
class wgetConnectErrorException : std::exception
{};
class wgetNotSupportException   : std::exception
{};





class wget
{
    std::string saveDir;
    
    int  delay;
    int  tries;
    bool recursive;
    bool noParent;
    bool pageRequist;
    
    hashTableURLs crawled;

public:
    wget (IN const std::string &saveDir,
          IN int  tries = 1,
          IN bool recursive = false,
          IN bool noParent = false,
          IN bool pageRequist = false,
          IN int  delay = 0) :
            saveDir (saveDir), delay (delay), tries (tries),
            recursive (recursive), noParent (noParent),
            pageRequist (pageRequist)
    {
        openlog ("wget_cli_log", LOG_PID | LOG_CONS, LOG_USER);
        syslog (LOG_INFO, "BEGIN wget");
        
        struct stat statsStruct = {};
        if ( stat (this->saveDir.c_str (), &statsStruct) != 0
             && (statsStruct.st_mode & S_IFDIR) == 0)
        {
            syslog (LOG_ERR, "ERROR save folder does not exist");
            throw wgetException ();
        }
        if ( this->saveDir.back() != kPathSeparator )
        { this->saveDir.push_back (kPathSeparator); }
        
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
    
    void download (IN const std::string &url,
                   IN       int   port_no=80,
                   IN const int   depth=1,
                   IN const bool  continueDump=false,
                   IN const bool  verbose=false);
        
protected:
    // return status_code
    int          splitHeader     (IN OUT std::string &responce,
                                  OUT    std::string &url_re_locate,
                                  IN bool verbose);
    
    std::string  formGetRequest  (IN const std::string &url);
    std::string  formOutFileName (IN const std::string &url);
    void         dumpOutFile     (IN const std::string &filename,
                                  IN const std::string &responce);
    void         parseHTML       (IN const std::string &html,
                                  OUT hashTableURLs &urls);
};

#endif // WGET_CLIENT_CRAWLER_H
