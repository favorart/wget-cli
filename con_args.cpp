#include "header.h"
// Created by favorart on 15.05.17.
//
#include "../wget/con_args.h"
#include <cstdio>

// #define __DEBUG


void  parseConsoleArguments (IN int argc, IN char **argv, OUT appArgs &args)
{
    const char *help_text =
            "\n console call:  wget-cli <url>                                 \n"
            "\t-l  (--level=<depth>)                                          \n"
            "\t-t  (--tries=<tries>)                                          \n"
            "\t-r  (--recursive)                                              \n"
            "\t-n  (--no-parent)        не выше данного адреса                \n"
            "\t-i  (--input-file=<fn>)  файл со ссылками                      \n"
            "\t-s  (--save-dir=<dir>)   каталог, куда сохранять файлы         \n"
            "\t-c  (--continue)         для продолжения оборвавшегося         \n"
            "\t-p  (--page-requisites)  ресурсы со страницы (css и картинки)  \n";
    
    const char *args_pattern = "u:l:t:rni:s:cpvh?";
    static struct option long_options[] =
    {
            {"url",             required_argument, 0, 'u'},
            {"level",           required_argument, 0, 'l'},
            {"tries",           required_argument, 0, 't'},
            {"recursive",             no_argument, 0, 'r'},
            {"no-parent",             no_argument, 0, 'n'},
            {"input-file",      required_argument, 0, 'i'},
            {"save-dir",        required_argument, 0, 's'},
            {"continue",              no_argument, 0, 'c'},
            {"page-requisites",       no_argument, 0, 'p'},
            {"verbose",               no_argument, 0, 'v'},
            {"help",                  no_argument, 0, 'h'},
            {"help",                  no_argument, 0, '?'},
            { 0, 0,                                0,  0 }
    };
    //-----------------------
    int c, option_index = 0;
    
    while ( -1 != (c = getopt_long (argc, argv, args_pattern,
                                    long_options, &option_index)))
    {
        switch ( c )
        {
            case -1:
               // no more arguments
               break;
            
            case 0:
#ifdef __DEBUG
                std::printf ("option %s", long_options[option_index].name);
                if ( optarg )
                    std::printf (" with arg %s", optarg);
                std::printf ("\n");
#endif // __DEBUG
                //-----------------------
                switch ( option_index )
                {
                    case 0: args.url         = optarg;        break;
                    case 1: args.depthLevel  = atoi (optarg); break;
                    case 2: args.triesNumber = atoi (optarg); break;
                    case 3: args.recursive   = true;          break;
                    case 4: args.noParent    = true;          break;
                    case 5: args.inputFile   = optarg;        break;
                    case 6: args.saveDir     = optarg;        break;
                    case 7: args.continLoad  = true;          break;
                    case 8: args.pageRequist = true;          break;
                    case 9: args.verbose     = true;          break;
                    default:
                        std::printf ("%s\n", help_text);
                        exit (1);
                        break;
                }
                break;
    
            case 'u': args.url         = optarg;        break;
            case 'l': args.depthLevel  = atoi (optarg); break;
            case 't': args.triesNumber = atoi (optarg); break;
            case 'r': args.recursive   = true;          break;
            case 'n': args.noParent    = true;          break;
            case 'i': args.inputFile   = optarg;        break;
            case 's': args.saveDir     = optarg;        break;
            case 'c': args.continLoad  = true;          break;
            case 'p': args.pageRequist = true;          break;
            case 'v': args.verbose     = true;          break;
            
            case '?': case 'h': default:
                /* getopt_long already printed an error message. */
                std::printf ("%s\n", help_text);
                exit (1);
                break;
        }
    }
    //-----------------------
#ifdef __DEBUG
    if ( optind < argc )
    {
        std::printf ("non-option ARGV-elements: ");
        while ( optind < argc )
            std::printf ("%s ", argv[optind++]);
        std::printf ("\n");
    }
    
    std::printf ("\n\n");
    std::printf ("%s\n", args.        url.c_str ());
    std::printf ("%d\n", args. depthLevel);           // -l  (--level=<depth>)
    std::printf ("%d\n", args.triesNumber);           // -t  (--tries=<tries>)
    std::printf ("%d\n", args.  recursive);           // -r  (--recursive)
    std::printf ("%d\n", args.   noParent);           // -n  (--no-parent)         не выше данного адреса
    std::printf ("%s\n", args.  inputFile.c_str ());  // -i  (--input-file=<dir>)  файл со ссылками
    std::printf ("%d\n", args. continLoad);           // -c  (--continue)          для продолжения оборвавшегося
    std::printf ("%d\n", args.pageRequist);           // -p  (--page-requisites)   ресурсы со страницы (css и картинки)
    std::printf ("%s\n", args.    saveDir.c_str ());  // -s  (--save-dir)          каталог, куда сохранять файлы
    std::printf ("%d\n", args.    verbose);           // -v  (--verbose)
#endif // __DEBUG
    //-----------------------
    return;
}
//-----------------------------------------

