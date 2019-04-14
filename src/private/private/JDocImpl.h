// Copyright (c) Matthew James Briggs

#pragma once

#include "ezxml/JDoc.h"

#include <iostream>
#include <memory>

namespace ezxml
{
    class JDocImpl : public JDoc
    {
    public:
        virtual ~JDocImpl() = default;
        JDocImpl( std::string inAttributePrefix,
                  std::string inArrayItemElement,
                  std::string inRootElement );

        // these can throw std::runtime_error
        virtual void loadStream( std::istream& is ) override;
        virtual void saveStream( std::ostream& os ) const override;

        // these can throw std::runtime_error
        virtual void loadFile( const std::string& filename ) override;
        virtual void saveFile( const std::string& filename ) const override;

        // AttributePrefix is used to distinguish attributes from elements
        virtual const std::string& getAttributePrefix() const override;

        // ArrayItemElement is used for nameless JSON array items
        virtual const std::string& getArrayItemName() const override;

        // Xml Interop
        virtual void fromXDoc( const XDocCPtr& inXDoc ) override;
        virtual XDocPtr toXDoc() const override;
        virtual XElementPtr getRoot() const override;
    
    private:
        std::string myAttributePrefix;
        std::string myArrayItemElement;
        std::string myRootElement;
        XDocPtr myXDoc;
    };
}
