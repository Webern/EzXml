#include "ezxml/ezxml.h"

namespace ezxmltest
{
    inline std::string
    elementsEqual( const std::string& inParentName, const ezxml::XElementPtr& a, const ezxml::XElementPtr& b );

    inline std::string
    elementsEqual(
            const std::string& inParentName,
            ezxml::XAttributeIterator aitera,
            const ezxml::XAttributeIterator& aenda,
            ezxml::XAttributeIterator aiterb,
            const ezxml::XAttributeIterator& aendb
    );


    inline std::string
    elementsEqual(
            const std::string& inParentName,
            ezxml::XElementIterator eitera,
            const ezxml::XElementIterator& eenda,
            ezxml::XElementIterator eiterb,
            const ezxml::XElementIterator& eendb
    )
    {
        // TODO - iterate each and call elementsEqual for each
    }


    inline std::string
    elementsEqual( const std::string& inParentName, const ezxml::XElementPtr& a, const ezxml::XElementPtr& b )
    {
        if( a == nullptr || b == nullptr )
        {
            return "cannot proceed due to null xelement";
        }

        if( a->getName() != b->getName() )
        {
            return inParentName + ": element names not equal: " + a->getName() + " vs " + b->getName();
        }

        if( a->getType() != b->getType() )
        {
            return inParentName
                   + "->"
                   + a->getName()
                   + ": element types not equal: "
                   + ezxml::toString( a->getType() ) + " vs "
                   + ezxml::toString( b->getType() );
        }

        // TODO - iterate each attribute and compare

        // TODO - iterate child attributes and compare
        return "";
    }


    std::string
    equal( const ezxml::XDocPtr& a, const ezxml::XDocPtr& b )
    {
        using namespace ezxml;

        if( a == nullptr || b == nullptr )
        {
            return "cannot proceed due to null xdoc";
        }

        if( a->getXmlVersion() != b->getXmlVersion() )
        {
            return "xml-versions not equal";
        }

        if( a->getEncoding() != b->getEncoding() )
        {
            return "getEncoding not equal";
        }

        if( a->getHasStandaloneAttribute() != b->getHasStandaloneAttribute() )
        {
            return "getHasStandaloneAttribute not equal";
        }

        if( a->getIsStandalone() != b->getIsStandalone() )
        {
            return "getIsStandalone not equal";
        }

        if( a->getHasDoctypeDeclaration() != b->getHasDoctypeDeclaration() )
        {
            return "getHasDoctypeDeclaration not equal";
        }

        if( a->getDoctypeValue() != b->getDoctypeValue() )
        {
            return "getDoctypeValue not equal";
        }

        const auto roota = a->getRoot();
        const auto rootb = b->getRoot();

        if( roota == nullptr || rootb == nullptr )
        {
            return "cannot proceed due to a null root";
        }

        return "";
    }
}