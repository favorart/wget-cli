//
// Created by favorart on 09.06.17.
//
#include "header.h"

// ================================================================
void  lower_case (IN OUT std::string &str)
{
    std::locale loc;
    // std::tolower (CHAR, loc);
    std::transform (str.begin (), str.end (), str.begin (),
                    [&loc] (char c) { return std::tolower (c, loc); });
}
// ================================================================
std::string escape_string (IN const std::string &str)
{
    const std::regex  escape ("[.^$|()\\[\\]{}*+?\\\\]");
    const std::string replace_to ("\\\\&");
    return std::regex_replace (str, escape, replace_to,
                               std::regex_constants::match_default |
                               std::regex_constants::format_sed);
}
// ================================================================
void url_prepare (IN OUT std::string &url)
{
    lower_case (url);
    
    std::string delimiter = "://";
    size_t  len_pref = url.find (delimiter);
    if (len_pref == std::string::npos)
        len_pref = 0;
    else
        len_pref += delimiter.size ();
    
    url = url.substr (len_pref, url.size () - len_pref);
    // if ( url.find ("/") == std::string::npos )
    //     url.push_back ('/');
    url = "http://" + url;
}

// ================================================================
const std::regex urlRxp ("^(?:https?://)?(?:www\\.|m\\.)?([^/]+)([\\s\\S]*)$");

std::string url_host (IN const std::string &url)
{
    std::smatch xResults;
    std::regex_match (url,  xResults, urlRxp);
    // std::string host = xResults[1];
    // return host;
    return xResults[1]; // host
}
std::string url_path (IN const std::string &url)
{
    std::smatch xResults;
    if ( std::regex_match (url,  xResults, urlRxp) )
    {
        std::string path = xResults[2];
        // std::cout << path << std::endl;
        return ( !path.empty () ) ? path : "/"; // path
    }
    else
    { return "/"; }
}
// ================================================================
