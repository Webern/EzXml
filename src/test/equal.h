#include "ezxml/ezxml.h"

namespace ezxmltest
{
    /*
        virtual XmlVersion getXmlVersion() const = 0;
        virtual void setXmlVersion( XmlVersion value ) = 0;
        virtual Encoding getEncoding() const = 0;
        virtual void setEncoding( Encoding value ) = 0;
        virtual bool getHasStandaloneAttribute() const = 0;
        virtual void setHasStandaloneAttribute( bool value ) = 0;
        virtual bool getIsStandalone() const = 0;
        virtual void setIsStandalone( bool value ) = 0;

        // Doctype Declaration
        virtual bool getHasDoctypeDeclaration() const = 0;
        virtual void setHasDoctypeDeclaration( bool value ) = 0;
        virtual std::string getDoctypeValue() const = 0;
        virtual void setDoctypeValue( const std::string& value ) = 0;
     */
    std::string
    equal( const XDocPtr& a, const XDocPtr& b )
    {
        if( a->getXmlVersion() != b->getXmlVersion() )
        {
            return "xml-versions not equal"
        }

    }
}