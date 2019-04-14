// Copyright (c) Matthew James Briggs
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "ezxml/XFactory.h"
#include "ezxml/XAttribute.h"
#include "private/JDocImpl.h"
#include "private/Throw.h"

#include <string>
#include <sstream>

namespace ezxml
{
    JDocImpl::JDocImpl(
        std::string inRootElement,
        std::string inAttributePrefix,
        std::string inArrayItemElement )
    : myRootElement( std::move( inRootElement ) )
    , myAttributePrefix{ std::move( inAttributePrefix ) }
    , myArrayItemElement{ std::move( inArrayItemElement ) }
    , myXDoc{ nullptr }
    {

    }

    void JDocImpl::convert(
        XElementPtr& xparent,
        const rapidjson::Value& jvalToAdd )
    {
        const auto t = jvalToAdd.GetType();

        switch( t )
        {
            case rapidjson::Type::kArrayType:
            {
                const auto& arr = jvalToAdd.GetArray();
                auto i = arr.Begin();
                const auto e = arr.End();

                for( ; i != e; ++i )
                {
                    auto newxel = xparent->appendChild( myArrayItemElement );
                    convert( newxel, *i );
                }
            }
            case rapidjson::Type::kObjectType:
            {
                const auto& obj = jvalToAdd.GetObject();
                auto i = obj.begin();
                const auto e = obj.end();

                for( ; i != e; ++i )
                {
                    const auto& name = std::string{ i->name.GetString() };
                    const auto& val = i->value;
                    const auto isAttribute =
                        name.size() >= myAttributePrefix.size() &&
                        name.substr( 0, myAttributePrefix.size() ) == myAttributePrefix;
                    const auto thisType = val.GetType();

                    std::string strVal = "";

                    if( thisType == rapidjson::Type::kStringType )
                    {
                        strVal = val.GetString();
                    }
                    else if( thisType == rapidjson::Type::kNumberType )
                    {
                        if( val.IsInt() )
                        {
                            strVal = std::to_string( val.GetInt() );
                        }
                        else if( val.IsInt64() )
                        {
                            strVal = std::to_string( val.GetInt64() );
                        }
                        else if( val.IsDouble() )
                        {
                            strVal = std::to_string( val.GetDouble() );
                        }
                        else
                        {
                            strVal = val.GetString();
                        }
                    }
                    else if( thisType == rapidjson::kTrueType )
                    {
                        strVal = "true";
                    }
                    else if( thisType == rapidjson::kFalseType )
                    {
                        strVal = "false";
                    }

                    if( isAttribute )
                    {
                        if( thisType == rapidjson::Type::kObjectType )
                        {
                            EZXML_THROW( "an attribute cannot be an object" );
                        }
                        else if( thisType == rapidjson::Type::kArrayType )
                        {
                            EZXML_THROW( "an attribute cannot be an array" );
                        }

                        auto attr = xparent->appendAttribute( myAttributePrefix + name );
                        attr->setValue( strVal );
                    }
                    else
                    {
                        if( thisType == rapidjson::Type::kObjectType )
                        {
                            auto child = xparent->appendChild( name );
                            convert( child, val );
                        }
                        else if( thisType == rapidjson::Type::kArrayType )
                        {
                            auto child = xparent->appendChild( name );
                            convert( child, val );
                        }
                        else
                        {
                            auto child = xparent->appendChild( name );
                            child->setValue( strVal );
                        }
                    }



//                    auto newxel = xparent->appendChild( myArrayItemElement );
//                    convert( newxel, *i );
                }
            }
            case rapidjson::Type::kTrueType:
            {
                xparent->setValue( "true" );
            }
            case rapidjson::Type::kFalseType:
            {
                xparent->setValue( "false" );
            }
            case rapidjson::Type::kNullType:
            {
                xparent->setValue( "" );
            }
            case rapidjson::Type::kNumberType:
            {
                if( jvalToAdd.IsInt() )
                {
                    xparent->setValue( std::to_string( jvalToAdd.GetInt() ) );
                }
                else if( jvalToAdd.IsInt64() )
                {
                    xparent->setValue( std::to_string( jvalToAdd.GetInt64() ) );
                }
                else if( jvalToAdd.IsDouble() )
                {
                    xparent->setValue( std::to_string( jvalToAdd.GetDouble() ) );
                }
                else
                {
                    xparent->setValue( jvalToAdd.GetString() );
                }
            }
            case rapidjson::Type::kStringType:
            {
                xparent->setValue( jvalToAdd.GetString() );
            }
        }
    }

    void JDocImpl::loadStream( std::istream& is )
    {
        myXDoc = XFactory::makeXDoc();
        std::string content{ std::istreambuf_iterator<char>( is ),
                             std::istreambuf_iterator<char>() };

        rapidjson::Document d;
        d.Parse( content.c_str() );
//        const auto& jval = d.Is;
        auto xroot = myXDoc->getRoot();
        xroot->setName( myRootElement );
        convert( xroot, d );
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
