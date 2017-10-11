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

struct Req
{
    std::string name;
    std::string type;
    std::string story;
    std::string number;
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
};

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

inline Composition extractComposition( const ezxml::XElement& inElement )
{
    if( inElement.getName() != "composition" ) { throw ""; }

    Composition comp;
    auto iter = inElement.begin();
    const auto e = inElement.end();

    for( ; iter != e; ++iter )
    {
        CompositionEntry entry;

        if( iter->getName() == "pp" )
        {
            entry.isImage = false;
            entry.text = iter->getValue();
            entry.text = trim( entry.text );
        }
        else if( iter->getName() == "image" )
        {
            entry.isImage = true;
            entry.text = iter->getValue();
            entry.text = trim( entry.text );

            for( auto a = iter->attributesBegin(); a != iter->attributesEnd(); ++a )
            {
                if( a->getName() == "filename" )
                {
                    entry.image.filename = a->getValue();
                }
                else if( a->getName() == "width" )
                {
                    entry.image.width = a->getValue();
                }
                else if( a->getName() == "border" )
                {
                    entry.image.border = a->getValue();
                }
            }
        }
        else
        {
            throw "";
        }

        comp.compositionEntries.push_back( entry );
    }

    return comp;
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
                                if( groupIter->getName() == "composition" )
                                {
                                    auto composition = extractComposition( *groupIter );

                                    if( isFirst )
                                    {
                                        isFirst = false;
                                        data.intro = composition;
                                    }
                                    else
                                    {
                                        data.outro = composition;
                                    }

                                }
                                else if( groupIter->getName() == "requirement" )
                                {
                                    Req req;
                                    req.story = groupIter->getValue();

                                    for( auto attrIter = groupIter->attributesBegin(); attrIter != groupIter->attributesEnd(); ++attrIter )
                                    {
                                        if( attrIter->getName() == "number" )
                                        {
                                            req.number = attrIter->getValue();
                                        }
                                        else if( attrIter->getName() == "type" )
                                        {
                                            req.type = attrIter->getValue();
                                        }
                                        else if( attrIter->getName() == "name" )
                                        {
                                            req.name = attrIter->getValue();
                                        }
                                    }
                                    data.reqs.push_back( req );
                                }
                            } // for( ; groupIter != groupEnd; ++groupIter )

                            // data extracted now change the structure of the group

                            while( groupsIter->removeChild( "requirement" ) ) {}
                            while( groupsIter->removeChild( "composition" ) ) {}

                            if ( groupsIter->begin() != groupsIter->end() )
                            {
                                throw std::runtime_error{ "sucky" };
                            }

                            const auto intro = groupsIter->appendChild( "intro" );

                            for( const auto& e : data.intro.compositionEntries )
                            {
                                XElementPtr entry = nullptr;

                                if( e.isImage )
                                {
                                    entry = intro->appendChild( "image" );
                                    entry->appendChild( "filename" )->setValue( e.image.filename );
                                    entry->appendChild( "width" )->setValue( e.image.width );
                                    entry->appendChild( "border" )->setValue( e.image.border );
                                    entry->appendChild( "caption" )->setValue( trim( e.text ) );
                                    entry->appendChild( "name" );
                                }
                                else
                                {
                                    entry = intro->appendChild( "pp" );
                                    entry->setValue( trim( e.text ) );
                                }
                            }

                            const auto reqs = groupsIter->appendChild( "reqs" );

                            for( const auto& r : data.reqs )
                            {
                                const auto req = reqs->appendChild( "req" );
                                req->appendChild( "name" )->setValue( r.name );
                                req->appendChild( "type" )->setValue( r.type );
                                req->appendChild( "story" )->setValue( trim( r.story ) );

                                auto points = req->appendChild( "points" );
                                points->setValue( "1" );
                                points->appendAttribute( "tag" )->setValue( "ui" );

                                points = req->appendChild( "points" );
                                points->setValue( "1" );
                                points->appendAttribute( "tag" )->setValue( "backend" );

                                points = req->appendChild( "points" );
                                points->setValue( "0" );
                                points->appendAttribute( "tag" )->setValue( "integ" );
                            }

                            const auto outro = groupsIter->appendChild( "outro" );

                            for( const auto& e : data.outro.compositionEntries )
                            {
                                XElementPtr entry = nullptr;

                                if( e.isImage )
                                {
                                    entry = outro->appendChild( "image" );
                                    entry->appendChild( "filename" )->setValue( e.image.filename );
                                    entry->appendChild( "width" )->setValue( e.image.width );
                                    entry->appendChild( "border" )->setValue( e.image.border );
                                    entry->appendChild( "caption" )->setValue( e.text );
                                    entry->appendChild( "name" );
                                }
                                else
                                {
                                    entry = outro->appendChild( "pp" );
                                    entry->setValue( e.text );
                                }
                            } // for( const auto& e : data.outro.compositionEntries )

                        } // for( ; groupsIter != groupsEnd; ++groupsIter )
                    } // if( epicIter->getName() == "groups" )
                } // for( ; epicIter != epicEnd; ++epicIter )
            } // for( ; docBodyIter != docBodyEnd; ++docBodyIter )
        }
    }

    std::stringstream ss;
    xdoc->saveStream( ss );
    std::cout << ss.str() << std::endl;

    return 0;
}
