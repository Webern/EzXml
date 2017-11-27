//
//  main.cpp
//  ParseSomething
//
//  Created by mjb on 6/12/17.
//  Copyright © 2017 Matthew James BRiggs. All rights reserved.
//

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include "ezxml/ezxml.h"


#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

//
//Left trim
//
std::string trim_left(const std::string& str)
{
    if( str.size() == 0 )
    {
        return str;
    }

    const std::string pattern = " \f\n\r\t\v";
    return str.substr(str.find_first_not_of(pattern));
}

//
//Right trim
//
std::string trim_right(const std::string& str)
{
    if( str.size() == 0 )
    {
        return str;
    }
    
    const std::string pattern = " \f\n\r\t\v";
    return str.substr(0,str.find_last_not_of(pattern) + 1);
}

//
//Left and Right trim
//
std::string trim(const std::string& str)
{
    if( str.size() == 0 )
    {
        return str;
    }

    return trim_left(trim_right(str));
}

int main(int argc, const char * argv[])
{
    using namespace ezxml;
    auto xdoc = XFactory::makeXDoc();
    const std::string oldFile = "/Users/mjb/MxRepo/Resources/mjbsuite/freezing.old.xml";
    const std::string newFile = "/Users/mjb/MxRepo/Resources/mjbsuite/freezing.xml";
    xdoc->loadFile( oldFile );
    const auto root = xdoc->getRoot();
    auto rootIter = root->begin();
    const auto rootEnd = root->end();

    for( ; rootIter != rootEnd; ++rootIter )
    {
        if( rootIter->getName() == "part" )
        {
            // new part
            int absoluteMeasureIndex = 0;
            int movementIndex = 0;
            int insideMovementMeasureIndex = 0;
            
            auto measure = rootIter->begin();
            const auto measuresEnd = rootIter->end();
            
            for( ; measure != measuresEnd; ++measure, ++absoluteMeasureIndex )
            {
                if( measure->getName() != "measure" )
                {
                    throw "bad";
                }
                
                // remove the implicit attribute
                for( auto attrIter = measure->attributesBegin(); attrIter != measure->attributesEnd(); ++attrIter )
                {
                    if( attrIter->getName() == "implicit" )
                    {
                        measure->removeAttribute( attrIter );
                        break;
                    }
                }
                
                for( auto attrIter = measure->attributesBegin(); attrIter != measure->attributesEnd(); ++attrIter )
                {
                    if( attrIter->getName() == "number" )
                    {
                        std::stringstream mnss;
                        mnss << "INDEX" << absoluteMeasureIndex << "_";
                        mnss << "MOVEMENT" << movementIndex + 1 << "_";
                        mnss << "MEASURE" << insideMovementMeasureIndex + 1;
                        attrIter->setValue( mnss.str() );
                    }
                }
                
                
                if( movementIndex == 0 && insideMovementMeasureIndex == 25 )
                {
                    ++movementIndex;
                    insideMovementMeasureIndex = 0;
                }
                else if( movementIndex == 1 && insideMovementMeasureIndex == 18 )
                {
                    ++movementIndex;
                    insideMovementMeasureIndex = 0;
                }
                else if( movementIndex == 2 && insideMovementMeasureIndex == 33 )
                {
                    ++movementIndex;
                    insideMovementMeasureIndex = 0;
                }
                else if( movementIndex == 3 && insideMovementMeasureIndex == 28 )
                {
                    ++movementIndex;
                    insideMovementMeasureIndex = 0;
                }
                else
                {
                    ++insideMovementMeasureIndex;
                }

            }

        } // if rootIter == "score-partwise"
    }

    xdoc->saveFile( newFile );
//    std::stringstream ss;
//    xdoc->saveStream( ss );
//    std::cout << ss.str() << std::endl;

    return 0;
}
