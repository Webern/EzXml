//
//  main.cpp
//  ParseSomething
//
//  Created by mjb on 6/12/17.
//  Copyright © 2017 Matthew James BRiggs. All rights reserved.
//

#include <iostream>
#include "ezxml/ezxml.h"

struct Req
{
    std::string name;
    std::string type;
    std::string story;
    std::string uiPoints;
    std::string backendPoints;
    std::string integPoints;
};

struct Image
{
    std::string filename;
    std::string border;
    std::string width;
};

struct CompositionEntry
{
    bool isImage;
    Image image;
    std::string text;
};

struct Composition
{
    std::vector<CompositionEntry> compositionEntries;
};

struct RequirementsData
{
    Composition intro;
    std::vector<Req> reqs;
    Composition outro;
}

int main(int argc, const char * argv[])
{
    using namespace ezxml;
    auto xdoc = XFactory::makeXDoc();
    xdoc->loadFile( "/Users/mjb/iqreq/data/requirements.xml" );
    const auto root = xdoc->getRoot();
    auto rootIter = root->begin();
    const auto rootEnd = root->end();

//    auto newdoc = XFactory::makeXDoc();
//    newdoc->setXmlVersion( xdoc->getXmlVersion() );
//    newdoc->setEncoding( xdoc->getEncoding() );
//    newdoc->setHasDoctypeDeclaration( false );
//    auto newRoot = newdoc->getRoot();
//    newRoot->setName( "document" );
//    newRoot->appendAttribute( "xmlns" )->setValue( "requirements" );
//    newRoot->appendAttribute( "xmlns:xsi" )->setValue( "http://www.w3.org/2001/XMLSchema-instance" );
//    newRoot->appendAttribute( "xsi:schemaLocation" )->setValue( "http://fakeurl.com requirements.xsd" );

    for( ; rootIter != rootEnd; ++rootIter )
    {
        if( rootIter->getName() == "document-body" )
        {
            const auto documentBodyElement = rootIter;
            auto docBodyIter = documentBodyElement->begin();
            const auto docBodyEnd = documentBodyElement->end();

            for( ; docBodyIter != docBodyEnd; ++docBodyIter )
            {
                if( docBodyIter->getName() != "epic" ) { throw std::runtime_error{ "bad element" }; }
                const auto epicElement = docBodyIter;
                auto epicIter = epicElement->begin();
                const auto epicEnd = epicElement->end();

                for( ; epicIter != epicEnd; ++epicIter )
                {
                    if( epicIter->getName() == "groups" )
                    {
                        const auto groupsElement = epicIter;
                        auto groupsIter = groupsElement->begin();
                        const auto groupsEnd = groupsElement->end();

                        for( ; groupsIter != groupsEnd; ++groupsIter )
                        {
                            if( groupsIter->getName() != "group" ) { throw std::runtime_error{ "bad element" }; }
                            const auto groupElement = groupsIter;
                            auto groupIter = groupElement->begin();
                            const auto groupEnd = groupElement->end();
                            bool isFirst = true;
                            RequirementsData data;

                            for( ; groupIter != groupEnd; ++groupIter )
                            {
                                if( isFirst )
                                {
                                    isFirst = false;
                                    if( groupIter->getName() == "composition" )
                                    {
                                        data.intro.isImage = groupIter->begin()->getName()
                                    }
                                }
                                std::cout << groupIter->getName() << std::endl;
                                
                            }
                        }
                    }
                }
            }
        }
    }

    std::stringstream ss;
    newdoc->saveStream( ss );
    std::cout << ss.str() << std::endl;
    return 0;
}
