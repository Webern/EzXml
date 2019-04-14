// Copyright (c) Matthew James Briggs

#pragma once

#include "ezxml/XElement.h"
#include "ezxml/XDoc.h"

#include <iostream>
#include <memory>

namespace ezxml
{
    class JDoc;
    using JDocPtr = std::shared_ptr<JDoc>;
    using JDocCPtr = std::shared_ptr<const JDoc>;

    // JDoc reads and writes JSON which can be translated to and from XML.
    // In order to support lossless round-tripping between XML and JSON,
    // JDoc takes an argument named 'inAttributePrefix' upon construction.
    // This will be prefixed to attributes to distinguish them from
    // elements in JSON. Also required is 'inArrayItemElement' which is
    // the element name that will be used for nameless JSON array items.
    class JDoc : public std::enable_shared_from_this<JDoc>
    {
    public:
        virtual ~JDoc() = default;

        // these can throw std::runtime_error
        virtual void loadStream( std::istream& is ) = 0;
        virtual void saveStream( std::ostream& os ) const = 0;

        // these can throw std::runtime_error
        virtual void loadFile( const std::string& filename ) = 0;
        virtual void saveFile( const std::string& filename ) const = 0;

        // AttributePrefix is used to distinguish attributes from elements
        virtual const std::string& getAttributePrefix() const = 0;

        // ArrayItemElement is used for nameless JSON array items
        virtual const std::string& getArrayItemName() const = 0;

        // Xml Interop
        virtual void fromXDoc( const XDocCPtr& inXDoc ) = 0;
        virtual XDocPtr toXDoc() const = 0;
        virtual XElementPtr getRoot() const = 0;
    };
}
