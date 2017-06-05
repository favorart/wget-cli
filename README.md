# wget-cli
The simplest recursive http-downloader.

con_args.h, con_args.cpp  - the parsing of console arguments
crawler.h, crawler.cpp    - Class wget of the http-downloading
parse_html.cpp            - the function of the html parsing
header.h, main.cpp        - the application call
	
Usage:
Call wget-cli

    -u --url=<url>        (std::string) = the http-address to download web-page                                                   
    -l --level=<depth>    (int)         = the depth level of recursive downloading the site
    -t --tries=<tries>    (int)         = number of tries to load a page, if error exists 
    -r --recursive        (bool)        = load pages by links found in the download pages 
    -n --no-parent        (bool)        = load pages of address not higher then given one
    -i --input-file=<dir> (std::string) = path to input text file with list of http-links   
    -c --continue         (bool)        = path to dir, where to save the html-pages         
    -p --page-requisites  (bool)        = continue an interrupted loading    (!not-realized yet)
    -s --save-dir         (std::string  = load page resourses (css and pics) (!not-realized yet)
    -v --verbose          (bool)        = print to stdout the steps of loading process      
    -h --help             (bool)        =  show the usage (the console args) 
    
