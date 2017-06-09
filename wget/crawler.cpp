#include "header.h"
#include "crawler.h"

// ================================================================
std::string  wget::formGetRequest  (IN const std::string &url)
{
    std::string request = "GET ";
    request.append (url_path (url));
    request.append (" HTTP/1.1\r\n");
    request.append ("User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\n");
    // Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:53.0) Gecko/20100101 Firefox/53.0;)
    request.append ("Accept: text/html, text/xml\r\n");
    // request.append ("Accept: text/html,application/xhtml+xml,application/xml\r\n");
    request.append ("Accept-Language: en-us, ru-ru\r\n");
    // request.append ("Accept-Encoding: identity, *;q=0\r\n");
    // request.append ("Accept-Language: ru,en-us;q=0.7,en;q=0.3\r\n");
    // request.append ("Accept-Charset: windows-1251,utf-8;q=0.7,*;q=0.7\r\n");
    request.append ("Host: ");
    request.append (url_host (url));
    request.append ("\r\n");
    request.append ("Connection: close\r\n");
    request.append ("\r\n");
    return request;
} // End of the function //

std::string  wget::formOutFileName (IN const std::string &url)
{
    std::string filename;
    const std::regex rmv_all ("[^a-zA-Z0-9]+");
    filename = std::regex_replace (url_host (url) + url_path (url),
                                   rmv_all, "_");
    
    if ( filename.front () == '_' )
        filename = filename.substr (1);
    if ( filename.back ()  == '_' )
        filename = filename.substr (0, filename.size () - 1);
    return  filename;
} // End of the function //

void  wget::dumpOutFile (IN const std::string &filename,
                         IN const std::string &responce)
{
    // dirs paths concat
    std::string  fn;
    fn.append (saveDir);
    fn.append (filename);
    
    std::ofstream  outFile (fn.c_str (), std::ios::out);
    outFile << responce << std::endl;
    outFile.close ();
} // End of the function //

int   wget::splitHeader (IN OUT std::string &responce,
                         OUT std::string &url_re_locate,
                         IN bool verbose)
{
    const std::regex respRxp (".*(HTTP[/]1\\.[10]\\s+(\\d+)[\\s\\S]*?\\r\\n\\r\\n)");
    std::smatch xResults;
    std::regex_search (responce,  xResults, respRxp);
    std::string header = xResults[1];
    
    if ( verbose )
    {
        std::cout << "Header:" << header << std::endl;
        std::cout << "---------------------------" << std::endl;
    }
    
    int status_code = std::stoi (xResults[2]);
    if ( 200 != status_code )
    { syslog (LOG_ERR, "ERROR not received page, status_code != 200."); }
    
    if ( 301 == status_code )
    {
        std::string location = "Location: ";
        std::size_t  s_pos = header.find (location);
        if ( s_pos != std::string::npos )
        {
            std::size_t  f_pos = header.find("\r\n", s_pos);
            if ( f_pos != std::string::npos )
            {
                s_pos += location.size ();
                url_re_locate = header.substr (s_pos, f_pos - s_pos);
            }
        }
    }
    
    responce = responce.substr (header.size (), responce.size () - header.size ());
    return status_code;
}

void  wget::download (IN const std::string &url,
                      IN const int  port_no,
                      IN const int  depth,
                      IN const bool continueDump,
                      IN const bool verbose)
{
    if ( depth > 0 )
    {
        int status = 0;
        int status_code = 0;
        int these_tries = tries;
        std::string _url = url;
        // ----------------------------------------------------
        do
        {
            std::string responce = "";
            // ----------------------------------------------------
            // Connection wiil be closed
            std::string request = formGetRequest (_url);
            if ( verbose )
            {
                std::cout << "Request: " << request << std::endl;
                std::cout << "=========================" << std::endl;
            }
            
            // const char *host = url_host (_url).c_str ();
            // status = socket_connect_and_receive (host, port, &request, &responce);
    
            status = socket_connect_send_and_receive (_url, port_no, request,
                                                      responce, MAX_DATA_SIZE, verbose);
            
            if ( !status && !responce.empty () )
            {
                std::string url_re_locate = "";
                status_code = splitHeader (responce, _url, verbose);
                if ( !url_re_locate.empty () )
                {
                    crawled.insert (_url);
                    _url = url_re_locate;
                    url_prepare (_url);
                }
                
                if ( verbose )
                {
                    // std::cout << "Response:" << responce << std::endl;
                    // std::cout << "---------------------------" << std::endl;
                }
            }
            if ( status_code == 200 )
            {
    
                std::string filename = formOutFileName (_url);
                dumpOutFile (filename, responce);
                if ( verbose )
                {
                    std::cout << "Writing to file : " << filename << std::endl;
                    std::cout << "---------------------------" << std::endl;
                }
    
                // ----------------------------------------------------
                crawled.insert (_url);
                // ----------------------------------------------------
                if ( recursive && depth > 1 )
                {
                    std::vector<std::string> v_urls;
                    {
                        hashTableURLs urls;
                        parseHTML (responce, urls);
                        v_urls = std::vector<std::string> (begin (urls), end (urls));
                    }
        
                    if ( noParent )
                    {
                        // parent url
                        std::string parentURL = url;
                        // LET IT BE THE ORIGINAL URL, THIS MY CHOISE !!!
                        
                        std::string  parentURLescaped = escape_string (parentURL);
                        std::regex parentRgx ("^" + parentURLescaped);
            
                        for ( auto it = v_urls.begin (); it != v_urls.end (); )
                        {
                            if ( !std::regex_search (*it, parentRgx) && (*it).front () != '/' )
                            {
                                it = v_urls.erase (it);
                            } // previously this was something like m_map.erase(it++)
                            else if ( (*it).front () == '/' ) // internal link
                            {
                                std::string new_url = parentURL;
                                if ( parentURL.back () == '/' )
                                    new_url.pop_back ();
                                // construct path
                                new_url += (*it);
                                
                                // // last char is not '/'
                                // if ( new_url.back () != '/' )
                                //   new_url.push_back ('/');
                    
                                *it = new_url;
                                ++it;
                            }
                            else
                            { ++it; }
                        }
                    }
        
                    if ( verbose )
                    {
                        std::cout << "Search for urls in html : " << std::endl;
                        for ( auto url : v_urls )
                            std::cout << url << std::endl;
                        std::cout << "---------------------------" << std::endl;
            
                    }
        
                    for ( auto url : v_urls )
                    {
                        if ( crawled.find (url) == crawled.end ())
                        {
                            sleep (delay);
                            download (url, 80, depth - 1, false, verbose);
                            // crawled.insert (url);
                        }
                    }
                } // end if reucsive
            }
        }
        while ( --these_tries && (status
                || (status_code != 200 && status_code != 404)) );
        // { throw wgetException (); }
    } // end if depth
}
// ================================================================
