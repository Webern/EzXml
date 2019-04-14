// Copyright (c) Matthew James Briggs

#include "ezxml/XFactory.h"
#include "private/PugiDoc.h"
#include "private/PugiElement.h"
#include "private/JDocImpl.h"

namespace ezxml
{
    JDocPtr XFactory::makeJDoc(
        std::string inRootElement,
        std::string inAttributePrefix,
        std::string inArrayItemElement )
    {
        return std::make_shared<JDocImpl>(
            std::move( inRootElement ),
            std::move( inAttributePrefix ),
            std::move( inArrayItemElement ) );
    }

    XDocPtr XFactory::makeXDoc()
    {
        return XDocPtr{ new PugiDoc{} };
    }
    
    XElementPtr XFactory::makeXElement()
    {
        return XElementPtr{ new PugiElement{} };
    }
}
