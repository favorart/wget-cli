//
// Created by favorart on 02.06.17.
//

#ifndef WGET_CLIENT_CON_ARGS_H
#define WGET_CLIENT_CON_ARGS_H

extern "C"
{
#include <getopt.h>   /* for getopt_long function */
}
#include <string>

struct appArgs
{
    std::string        url;  // v |
    int         depthLevel;  // v | -l  (--level=<depth>)
    int        triesNumber;  // v | -t  (--tries=<tries>)
    bool         recursive;  // v | -r  (--recursive)
    bool          noParent;  // v | -n  (--no-parent)         не выше данного адреса
    std::string  inputFile;  // v | -i  (--input-file=<dir>)  файл со ссылками
    bool        continLoad;  // - | -c  (--continue)          для продолжения оборвавшегося
    bool       pageRequist;  // - | -p  (--page-requisites)   ресурсы со страницы (css и картинки)
    std::string    saveDir;  // v | -s  (--save-dir)          каталог, куда сохранять файлы
    bool           verbose;  // v | -v  (--verbose)
};
void  parseConsoleArguments (int argc, char **argv, appArgs &args);

#endif //WGET_CLIENT_CON_ARGS_H
