//
// Created by favorart on 02.06.17.
//
#include "../wget/header.h"
#include "../wget/con_args.h"
#include "../wget/crawler.h"

#include <boost/network/protocol/http/client.hpp>
namespace http = boost::network::http;
namespace  uri = boost::network::uri;
namespace   fs = boost::filesystem;


int main (int argc, char **argv)
{
    appArgs args = {};
    parseConsoleArguments (argc, argv, args);
    
    std::cout << "Launching program" << std::endl;
    auto  getter = wget (args.saveDir, args.triesNumber,
                         args.recursive, args.noParent, args.pageRequist);
    
    if ( !args.url.empty () )
    {
        getter.download (uri::uri (args.url), 80, args.depthLevel,
                         args.continLoad, args.verbose);
        
        if ( args.verbose )
        { std::cout << "Done" << std::endl; }
    }
    else if ( !args.inputFile.empty () )
    {
        std::ifstream   f_in (args.inputFile);
        for (std::string url; getline (f_in, url); )
        {
            getter.download (uri::uri (url), 80, args.depthLevel,
                             args.continLoad, args.verbose);
        }
        if ( args.verbose )
        { std::cout << "Done" << std::endl; }
    }

    // std::string result = get_http_data ("www.open-std.org/jtc1/sc22/wg21/docs/papers/2011/n3242.pdf");
    // std::ofstream of ("cpp11_draft_n3242.pdf", std::ios::binary);
    // of << result;
    
    return 0;
}
