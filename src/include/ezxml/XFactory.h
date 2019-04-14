// Copyright (c) Matthew James Briggs

#pragma once

#include "ezxml/XDoc.h"
#include "ezxml/JDoc.h"
#include <memory>

namespace ezxml
{
    class XFactory
    {
    public:
        static JDocPtr makeJDoc(
            std::string inRootElement,
            std::string inAttributePrefix,
            std::string inArrayItemElement );

        static XDocPtr makeXDoc();
        static XElementPtr makeXElement();
    };
}
