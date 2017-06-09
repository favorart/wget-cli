//
// Created by favorart on 09.06.17.
//
#include "header.h"


bool socket_connect_send_and_receive (IN const std::string &url, IN int port_no,
                                      IN const std::string &request,
                                      OUT      std::string &responce, IN int size,
                                      IN bool  verbose)
{
    bool result = false;
    
    int m_sock;
    struct sockaddr_in serv_addr = {0};
    serv_addr.sin_family = AF_INET;
    
    int recv_size = -1;
    int on = 1; // setsockopt
    
    if (verbose)
        std::cout << "Host: " << url_host (url) << std::endl;

    serv_addr.sin_addr.s_addr = inet_addr (url_host (url).c_str ());
    if (serv_addr.sin_addr.s_addr == INADDR_NONE)
    {
        struct hostent *phost = gethostbyname (url_host (url).c_str ());
        if ((phost) && (phost->h_addrtype == AF_INET))
        {
            serv_addr.sin_addr = *(in_addr *) (phost->h_addr);
            if ( verbose )
                std::cout << "IP: " << inet_ntoa (serv_addr.sin_addr) << std::endl;
        }
        else
        {
            syslog (LOG_ERR, "ERROR: Could not find the IP");
            result = true;
            goto FREE;
            // return true;
        }
    }
        
    if ( (m_sock = socket (AF_INET, SOCK_STREAM, 0)) == -1 )
    {
        syslog (LOG_ERR, "ERROR: Could not create a socket");
        result = true;
        goto FREE;
        // return true;
    }
    
    if ( -1  == setsockopt (m_sock, SOL_SOCKET, SO_REUSEADDR,
                            (const char *) &on, sizeof (on)) )
    {
        syslog (LOG_ERR, "ERROR: SetSockOpt REUSE is failed");
        result = true;
        goto FREE;
        // close (m_sock);
        // return true;
    }
    
    if ( verbose )
        std::cout << "Socket is created" << std::endl;
    
    serv_addr.sin_port = htons (80);
    if ( connect (m_sock,(struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        syslog (LOG_ERR, "ERROR: Connecting");
        result = true;
        goto FREE;
        // close (m_sock);
        // return true;
    }
    
    syslog (LOG_INFO, "CONNETCTED");
    if ( verbose )
        std::cout << "Connected" << std::endl;
    
    if( send (m_sock , request.c_str () , request.size (), 0) < 0)
    {
        syslog (LOG_ERR, "ERROR: Sending to socket is failed");
        result = true;
        goto FREE;
        // close (m_sock);
        // return true;
    }
    
    if ( verbose )
        std::cout << "Request has been sended" << std::endl;
        
    // Receive a responce from the server
    do
    {
        char buffer[size] = {};
        // RECEIVING ....
        recv_size = (int)::recv (m_sock, buffer, size, 0);
        if ( recv_size < 0 )
        {
            syslog (LOG_ERR, "ERROR: receiving from socket is failed");
            result = true;
            goto FREE;
            // close (m_sock);
            // return true;
        }
        responce.append (buffer);
        
    } while ( recv_size != 0 ); // End of the while
    
    if ( verbose )
        std::cout << "Responce has been received\n" << std::endl;
    
FREE:
    close  (m_sock);
    syslog (LOG_INFO, "CONNETCTION CLOSED");
    return result;
} // End of the function //
