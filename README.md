# wget-cli
The simplest recursive http-downloader.

0. sites.txt = urls the app was started on 
1. GET (txt) = request tested by "nc -q 3 mail.ru 80 < GET"
2. wget-cli (.exe) = successful compiled version.
3. start.sh = example how may run the app
4. log.txt = console dump of start.sh
5. Makefile = manually created file (to be sure)
6. CMakeLists.txt = automatic created file by CLion (not exactly understand how it works)

There is no support file types [.pdf,.txt,.xml,.pic,.jpg,.zip etc]
Check only status code 200 (301 a little)

	* con_args.h, con_args.cpp  - the parsing of console arguments
	* crawler.h, crawler.cpp    - class wget of downloading web-pages
	* parse_html.cpp            - the function of the html parsing
	* header.h, main.cpp        - the application call
	
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
    
