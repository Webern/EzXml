// Copyright (c) Matthew James Briggs
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "ezxml/XFactory.h"
#include "private/JDocImpl.h"

#include <string>
#include <sstream>

namespace ezxml
{
    JDocImpl::JDocImpl(
        std::string inAttributePrefix,
        std::string inArrayItemElement,
        std::string inRootElement )
    : myAttributePrefix{ std::move( inAttributePrefix ) }
    , myArrayItemElement{ std::move( inArrayItemElement ) }
    , myRootElement( std::move( inRootElement ) )
    , myXDoc{ nullptr }
    {

    }

    static void convert( XElement& xel, rapidjson::Value& jval )
    {

    }

    void JDocImpl::loadStream( std::istream& is )
    {
        myXDoc = XFactory::makeXDoc();
        std::string content{ std::istreambuf_iterator<char>( is ),
                             std::istreambuf_iterator<char>() };

        rapidjson::Document d;
        d.Parse( content.c_str() );

        auto xroot = myXDoc->getRoot();
//        convert( xroot, jval );
    }


    void JDocImpl::saveStream( std::ostream& os ) const
    {

    }

    
    void JDocImpl::loadFile( const std::string& filename )
    {

    }

    
    void JDocImpl::saveFile( const std::string& filename ) const
    {

    }

    
    const std::string& JDocImpl::getAttributePrefix() const
    {
        return myAttributePrefix;
    }

    
    const std::string& JDocImpl::getArrayItemName() const
    {
        return myArrayItemElement;
    }

    
    void JDocImpl::fromXDoc( const XDocCPtr& inXDoc )
    {
        myXDoc = XFactory::makeXDoc();
    }

    
    XDocPtr JDocImpl::toXDoc() const
    {
        if( !myXDoc )
        {
            return XFactory::makeXDoc();
        }

        return myXDoc;
    }

    
    XElementPtr JDocImpl::getRoot() const
    {
        auto xdoc = toXDoc();
        return xdoc->getRoot();
    }
}
