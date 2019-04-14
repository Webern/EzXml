// Copyright (c) Matthew James Briggs

#include "ezxml/XFactory.h"
#include "private/PugiDoc.h"
#include "private/PugiElement.h"

namespace ezxml
{
    JDocPtr XFactory::makeJDoc()
    {
        return JDocPtr{ nullptr };
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
