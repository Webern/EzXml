//
//  main.cpp
//  ParseSomething
//
//  Created by mjb on 6/12/17.
//  Copyright © 2017 Matthew James BRiggs. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include "ezxml/ezxml.h"
#include "StringDistance.h"


const char* const inFilepathGmRef = "/Users/mjb/komp/Resources/Documents/DevNotes/GeneralMidiReference.xml";

struct Gm
{
    int number;
    std::string name;
    std::string cppName;
};

int main(int argc, const char * argv[]) {
    using namespace ezxml;
    auto xdoc = XFactory::makeXDoc();
    xdoc->loadFile( inFilepathGmRef );

    auto iter = xdoc->getRoot()->begin();
    const auto e = xdoc->getRoot()->end();

    std::vector<Gm> gms;

    for( ; iter != e; ++iter )
    {
        Gm gm;
        gm.number = std::stoi( iter->begin()->getValue() );
        auto child = iter->begin();
        ++child;
        gm.name = child->getValue();
        gms.push_back( gm );
    }

    for( auto& gm : gms )
    {
        std::stringstream ss;
        bool isSpacerAdded = false;

        for( const auto& c : gm.name )
        {
            if( std::isalnum( c ) )
            {
                isSpacerAdded = false;
                ss << static_cast<char>( std::toupper( c ) );
            }
            else if ( !isSpacerAdded )
            {
                ss << "_";
                isSpacerAdded = true;
            }
        }

        gm.cppName = ss.str();
        if( gm.cppName.back() == '_' )
        {
            gm.cppName = gm.cppName.substr(0, gm.cppName.length() - 1 );
        }
    }

//    std::cout << "constexpr const char* const GM_STRING_UNKNOWN" << " = \"UNKNOWN\";" << std::endl;
//
//    for( const auto& gm : gms )
//    {
//        std::cout << "constexpr const char* const GM_STRING_" << gm.cppName << " = \"" << gm.name << "\";" << std::endl;
//    }

    // case GeneralMidi::UNKNOWN: return GM_STRING_UNKNOWN;

//    for( const auto& gm : gms )
//    {
//        std::cout << "case GeneralMidi::" << gm.cppName << ": return GM_STRING_" << gm.cppName << ";" << std::endl;
//    }

    /*
     if( std::strcmp( inString, GM_STRING_UNKNOWN ) == 0 )
     {
     return GeneralMidi::UNKNOWN;
     }
     */

    for( const auto& gm : gms )
    {
        std::cout << "else if( std::strcmp( inString, GM_STRING_" << gm.cppName << " ) )" << std::endl;
        std::cout << "{" << std::endl;
        std::cout << "return GeneralMidi::" << gm.cppName << ";" << std::endl;
        std::cout << "}" << std::endl;
    }

    return 0;
}
