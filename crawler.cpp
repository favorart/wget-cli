#include "../wget/crawler.h"
namespace uri = boost::network::uri;
namespace  fs = boost::filesystem;


extern "C"
{
bool  socket_connect_and_receive (IN const char *host, IN int port,
                                  IN const std::string *request,
                                  OUT std::string *responce)
{
    bool result = false;
    
    int on = 1;
    int status = -1;
    // Setup the m_sock
    int          m_sock;
    sockaddr_in  m_addr;
    memset (&m_addr, 0, sizeof (m_addr));
    
    // struct addrinfo *ai;
    // struct addrinfo hints;
    // memset (&hints, 0, sizeof (struct addrinfo));
    // hints.ai_family   = AF_UNSPEC;
    // hints.ai_socktype = SOCK_STREAM;
    // hints.ai_protocol = IPPROTO_TCP;
    
    if ( m_sock = (int)socket (AF_INET, SOCK_STREAM, 0) < 0)
    {
        syslog (LOG_ERR, "ERROR opening socket.");
        result = true;
        goto FREE;
    }
    
    // struct hostent *server = gethostbyname (host);
    // if (server == NULL) {
    //    syslog (LOG_ERR, "ERROR, no such host\n");
    //    result = true;
    //    goto FREE;
    //}
    
    if ( -1  == setsockopt (m_sock, SOL_SOCKET, SO_REUSEADDR,
                            (const char *) &on, sizeof (on)) )
    {
        syslog (LOG_ERR, "ERROR setsockopt is failed");
        result = true;
        goto FREE;
    }
    // { throw wgetConnectErrorException (); }
    
    // Connect //
    m_addr.sin_family = AF_INET;
    m_addr.sin_port = htons (port);
    
    status = inet_pton (AF_INET, host, &m_addr.sin_addr);
    if ( errno == EAFNOSUPPORT )
    {
        syslog (LOG_ERR, "ERROR EAFNOSUPPORT");
        result = true;
        goto FREE;
    }
    // { throw wgetNotSupportException (); }
    
    status = (int)::connect (m_sock, (sockaddr *) &m_addr, sizeof (m_addr));
    if (status < 0)
    {
        syslog (LOG_ERR, "ERROR connecting");
        result = true;
        goto FREE;
    }
    
    syslog (LOG_INFO, "CONNETCTED");
    
    status = (int)::send (m_sock, request->c_str (), request->size (), MSG_NOSIGNAL);
    if (status < 0)
    {
        syslog (LOG_ERR, "ERROR sendng GET request.");
        result = true;
        goto FREE;
    }
    
    syslog (LOG_INFO, "GET %s HTTP/1.1\r\n", host);
    
    // RECEIVING ....
    char buf[MAXRECV];
    while ( status != 0 )
    {
        memset (buf, 0, MAXRECV);
        status = (int)::recv (m_sock, buf, MAXRECV, 0);
        if (status < 0)
        {
            syslog (LOG_ERR, "ERROR receiving from socket");
            result = true;
            goto FREE;
        }
        responce->append (buf);
    } // End of the while //
    
FREE:;
    close  (m_sock);
    syslog (LOG_INFO, "CONNETCTION CLOSED");
    return result;
} // End of the function //
}

// ================================================================
std::string  wget::formGetRequest  (IN const uri::uri &url)
{
    // std::cout << url.host () << ' ' << url.path () << std::endl;
    
    std::string request = "GET ";
    request.append (url.path ());
    request.append (" HTTP/1.0\r\n");
    request.append ("Host: ");
    request.append (url.host ());
    request.append ("\r\n");
    request.append ("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.81\r\n");
    request.append ("User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:53.0) Gecko/20100101 Firefox/53.0;)\r\n");
    request.append ("Connection: close\r\n");
    request.append ("\r\n");
    return request;
} // End of the function //

std::string  wget::formOutFileName (IN const uri::uri &url)
{
    std::string filename;
    const boost::regex rmv_all ("[^a-zA-Z0-9]+");
    filename = boost::regex_replace (url.host () + url.path (), rmv_all, "_");
    
    if ( boost::starts_with (filename, "_"))
        filename = filename.substr (1, filename.size () - 1);
    
    if ( boost::ends_with (filename, "_"))
        filename = filename.substr (0, filename.size () - 2);
    return  filename;
} // End of the function //

void  wget::dumpOutFile (IN const std::string &filename,
                         IN const std::string &responce)
{
    fs::path fn (filename);
    fs::path path = saveDir / fn;  // paths concat
    
    std::ofstream  outFile (path.c_str (), std::ios::out);
    outFile << responce << std::endl;
    outFile.close ();
} // End of the function //

bool  wget::splitHeader (IN OUT std::string &responce, IN bool verbose)
{
    const boost::regex respRxp ("^(HTTP/1\\.[10]\\s+(\\d+).*?\r\n\r\n).*");
    boost::smatch xResults;
    std::string header;
    int status_code;
    
    boost::regex_match (responce,  xResults, respRxp);
    header = xResults[1];
    status_code = std::stoi (xResults[2]);
    
    if ( verbose )
    {
        std::cout << "Header:" << header << std::endl;
        std::cout << "---------------------------" << std::endl;
    }
    
    if ( 200 != status_code )
    {
        syslog (LOG_ERR, "ERROR not received page, status_code != 200.");
        return true;
    }
    
    responce = responce.substr (header.size (), responce.size () - header.size ());
    return false;
}

void  wget::download (IN const uri::uri &url,
                      IN const int  port,
                      IN const int  depth,
                      IN const bool continueDump,
                      IN const bool verbose)
{
    if ( depth > 0 )
    {
        // HTTP/1.1 defines the "close" connection option for
        // the sender to signal that the connection will be closed
        // after completion of the response.
        std::string request = formGetRequest (url);
        
        if ( verbose )
        {
            std::cout << "Request: " << request << std::endl;
            std::cout << "=========================" << std::endl;
        }
        
        // End of building the request //
        std::string responce;
        const char *host = url.host ().c_str ();
        int status = 0;
        int these_tries = tries;
        do
        {
            responce = "";
            status = socket_connect_and_receive (host, port, &request, &responce);
            // status = fetch (url, port, request, responce);
            if ( !status )
            { status = splitHeader (responce, verbose); }
        }
        while ( status && --these_tries );
        // { throw wgetException (); }
        
        crawled.insert (url.string ());
        
        if ( verbose )
        {
            // std::cout << "Response:" << responce << std::endl;
            // std::cout << "---------------------------" << std::endl;
        }
        
        std::string filename = formOutFileName (url);
        dumpOutFile (filename, responce);
        
        if ( verbose )
        {
            std::cout << "Writing to file : " << filename << std::endl;
            std::cout << "---------------------------" << std::endl;
        }
        
        if ( recursive && depth > 1 )
        {
            std::vector<std::string> v_urls;
            {
                hashtableURIs urls;
                parseHTML (responce, urls);
                v_urls = std::vector<std::string> (begin (urls), end (urls));
            }
            
            if ( noParent )
            {
                // parent
                std::string parentURL = url.string ();
                boost::algorithm::to_lower (parentURL);
                std::string  parentURLescaped = escape_string (parentURL);
                boost::regex parentRgx ("^" + parentURLescaped);
    
                for ( auto it = v_urls.begin (); it != v_urls.end (); )
                {
                    // std::string  &url = *it;
                    if ( !boost::regex_match (*it, parentRgx) and (*it)[0] != '/' )
                    {
                        it = v_urls.erase (it);
                    }   // previously this was something like m_map.erase(it++)
                    else if ( (*it)[0] == '/' ) // internal link
                    {
                        std::string new_url;
                        if ( parentURL[parentURL.size () - 1] == '/' )
                            new_url =  parentURL + (*it).substr (1);
                        else
                            new_url =  parentURL + (*it);
                        // last char is not '/'
                        if ( new_url[new_url.size () - 1] != '/' )
                            *it += "/";
                        
                        *it = new_url;
                        ++it;
                    }
                    else
                    { ++it; }
                }
            }
            
            if ( verbose )
            {
                std::cout << "---------------------------" << std::endl;
                std::cout << "Serch for urls in html : " << std::endl;
                
                for ( auto url : v_urls )
                    std::cout << url << std::endl;
                
            }
            
            for ( auto url : v_urls )
            {
                if ( crawled.find (url) == crawled.end ())
                {
                    sleep (delay);
                    download (uri::uri (url), 80, depth - 1, false, verbose);
                    // crawled.insert (url);
                }
            }
        } // end if reucsive
    } // end if depth
} // End of the function //

// ================================================================
//void  wget::connect  (const uri::uri &url)
//{
//    boost::asio::io_service io_service ;
//    boost::asio::ip::tcp::resolver resolver(io_service) ;
//
//    boost::asio::ip::tcp::resolver::query query (url.string (), "http");
//    boost::asio::ip::tcp::resolver::iterator iter = resolver.resolve (query);
//    boost::asio::ip::tcp::resolver::iterator end;
//    boost::asio::ip::tcp::endpoint endpoint;
//    while (iter != end)
//    {
//        endpoint = *iter++;
//        std::cout << endpoint << std::endl;
//    }
//
//    boost::asio::ip::tcp::socket socket(io_service);
//    socket.connect(endpoint);
//
//    boost::asio::streambuf  request;
//    std::ostream  request_stream (&request);
//    request_stream << "GET / HTTP/1.0\r\n";
//    request_stream << "Host: localhost \r\n";
//    request_stream << "Accept: */*\r\n";
//    request_stream << "Connection: close\r\n\r\n";
//
//    boost::asio::write(socket, request);
//
//    boost::asio::streambuf header;
//    boost::asio::read_until (socket, header, "\r\n\r\n");
//
//    // std::cout << &response << std::endl;
//
//}
//
//bool  wget::fetch (IN  const uri::uri     &url,
//                   IN        int           port,
//                   IN  const std::string  &request,
//                   OUT       std::string  &responce)
//{
//    try
//    {
//        boost::asio::ip::tcp::iostream  net_stream (url.host (), "http");
//        net_stream.expires_from_now (boost::posix_time::seconds (60));
//
//        if ( !net_stream )
//        { throw "Unable to connect: " + net_stream.error ().message (); }
//
//        syslog (LOG_INFO, "CONNECTED");
//
//        // ask for the file
//        net_stream << "GET " << url.path () << " HTTP/1.0\r\n";
//        net_stream << "Host: " << url.host () << "\r\n";
//        net_stream << "Accept: */*\r\n";
//        net_stream << "Connection: close\r\n\r\n";
//
//        // Check that response is OK.
//        std::string   http_version;
//        net_stream >> http_version;
//
//        unsigned int  status_code;
//        net_stream >> status_code;
//
//        std::string   status_message;
//        std::getline (net_stream, status_message);
//
//        if ( !net_stream && http_version.substr (0, 5) != "HTTP/" )
//        { throw "Invalid response\n"; }
//
//        if (status_code != 200)
//        { throw "Response returned with status code " + status_code; }
//
//        // Process the response headers, which are terminated by a blank line.
//        std::string header;
//        while (std::getline (net_stream, header) && header != "\r") {}
//
//        // Write the remaining data to output.
//        std::stringstream ss;
//        ss << net_stream.rdbuf();
//        responce = ss.str();
//    }
//    catch (std::exception& e)
//    {
//        std::cout << e.what () << std::endl;
//        // syslog (LOG_ERR, e.what ());
//        return true;
//    }
//    return false;
//}
