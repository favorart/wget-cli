//
// Created by favorart on 02.06.17.
//
#include "header.h"
#include "con_args.h"
#include "crawler.h"

int main (int argc, char **argv)
{
    appArgs args = {};
    parseConsoleArguments (argc, argv, args);
    
    std::cout << "Launching program" << std::endl;
    auto  getter = wget (args.saveDir, args.triesNumber,
                         args.recursive, args.noParent, args.pageRequist);
    
    if ( !args.url.empty () )
    {
        std::string url = args.url;
        url_prepare (url);
        getter.download (url, 80, args.depthLevel,
                         args.continLoad, args.verbose);
        
        if ( args.verbose )
        { std::cout << "Done" << std::endl; }
    }
    else if ( !args.inputFile.empty () )
    {
        std::ifstream   f_in (args.inputFile);
        if ( f_in.is_open () )
        {
            for ( std::string url; getline (f_in, url); )
            {
                if ( !url.empty () )
                {
                    url_prepare (url);
                    getter.download (url, 80, args.depthLevel,
                                     args.continLoad, args.verbose);
                }
            }
        }
        if ( args.verbose )
        { std::cout << "Done" << std::endl; }
    }
    return 0;
}
