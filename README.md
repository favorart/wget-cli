# wget-cli
The simplest recursive http-downloader.

con_args.h, con_args.cpp 	- parsing of console arguments
crawler.h, crawler.cpp    - the main class of http-downloader
parse_html.cpp            - 
header.h 	init 	3 days ago
	main.cpp 	bug fix 	7 hours ago
	
Usage:
Call wget-cli

    -u --url=<url>        (std::string) =                                                   
    -l --level\=\<depth\>    (int)         = the http-address to download web-page             
    -t --tries\=\<tries\>    (int)         = the depth level of recursive downloading the site 
    -r --recursive        (bool)        = number of tries to load a page, if error exists   
    -n --no-parent        (bool)        = load pages by links found in the download pages   
    -i --input-file\=\<dir\> (std::string) = load pages of address not higher then given one   
    -c --continue         (bool)        = path to input text file with list of http-links   
    -p --page-requisites  (bool)        = path to dir, where to save the html-pages         
    -s --save-dir         (std::string  = continue an interrupted loading    (!not-realized yet)
    -v --verbose          (bool)        = load page resourses (css and pics) (!not-realized yet)
    -h --help             (bool)        = print to stdout the steps of loading process      
                                          show the usage (the console args)                 
