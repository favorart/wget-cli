#include "../wget/crawler.h"
#include <gumbo.h>


static void  searchForLinksInHTML (IN GumboNode* node,
                                   OUT std::unordered_set<std::string> &urls)
{
    if ( node->type != GUMBO_NODE_ELEMENT )
    { return; }
    
    GumboAttribute *href;
    if ( node->v.element.tag == GUMBO_TAG_A &&
         (href = gumbo_get_attribute (&node->v.element.attributes, "href")))
    {
        std::string  url = std::string (href->value);
        boost::algorithm::to_lower (url);
        urls.insert (url);
        // std::cout << href->value << std::endl;
    }
    
    GumboVector *children = &node->v.element.children;
    for ( unsigned int i = 0; i < children->length; ++i )
    { searchForLinksInHTML (static_cast<GumboNode*> (children->data[i]), urls); }
}

void  wget::parseHTML (IN  const std::string &html,
                       OUT std::unordered_set<std::string> &urls)
{
    GumboOutput *output = gumbo_parse (html.c_str ());
    searchForLinksInHTML (output->root, urls);
    gumbo_destroy_output (&kGumboDefaultOptions, output);
}
