//
//  main.cpp
//  ParseSomething
//
//  Created by mjb on 6/12/17.
//  Copyright © 2017 Matthew James BRiggs. All rights reserved.
//

#include <iostream>
#include "ezxml/ezxml.h"

int main(int argc, const char * argv[])
{
    using namespace ezxml;
    auto xdoc = XFactory::makeXDoc();
    xdoc->loadFile(  );
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

        std::stringstream upperCase;

        for( auto c = name.cbegin(); c != name.cend(); ++c )
        {
            if( *c == '.' || *c == '-' )
            {
                upperCase << "_";
                continue;
            }
            else
            {
                upperCase << static_cast<char>( std::toupper( ( *c ) ) );
            }
        }

//        std::cout << "ZEUS_ENUM_VALUE( "
//        << upperCase.str()
//        << ", \""
//        << name
//        << "\" ) \\"
//        << std::endl;

        std::cout << "std::make_pair<komp::SoundID, mx::api::SoundID>(komp::SoundID::"
        << upperCase.str()
        << ", mx::api::SoundID::"
        << camelCase.str()
        << "),"
        << std::endl;

//        std::cout << "    "
//        << camelCase.str()
//        << "," << std::endl;

//        std::cout << "std::pair<core::PlaybackSound, api::InstrumentSound>{ core::PlaybackSound::"
//        << camelCase.str() << ", api::InstrumentSound::"
//        << camelCase.str() << " },"
//        << std::endl;

    }
    return 0;
}
