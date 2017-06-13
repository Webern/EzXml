//
//  main.cpp
//  ParseSomething
//
//  Created by mjb on 6/12/17.
//  Copyright © 2017 Matthew James BRiggs. All rights reserved.
//

#include <iostream>
#include "ezxml/ezxml.h"

int main(int argc, const char * argv[]) {
    using namespace ezxml;
    auto xdoc = XFactory::makeXDoc();
    xdoc->loadFile( "/Users/mjb/Dropbox/Programming/MxRepo/Documents/DevNotes/MusicXML Standard Sounds.xml" );
    const auto root = xdoc->getRoot();
    auto iter = root->begin();
    const auto e = root->end();

    for( ; iter != e; ++iter )
    {
        const std::string name = iter->attributesBegin()->getValue();
        std::stringstream camelCase;
        bool nextIsUpper = false;
        for( auto c = name.cbegin(); c != name.cend(); ++c )
        {
            if( *c == '.' || *c == '-' )
            {
                nextIsUpper = true;
                continue;
            }
            else if( nextIsUpper )
            {
                camelCase << static_cast<char>( std::toupper( ( *c ) ) );
            }
            else
            {
                camelCase << ( *c );
            }
            nextIsUpper = false;
        }

        std::cout << camelCase.str() << std::endl;
    }
    return 0;
}
