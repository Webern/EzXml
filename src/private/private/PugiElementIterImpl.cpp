// Copyright (c) Matthew James Briggs

#include "private/PugiElementIterImpl.h"
#include "private/XThrow.h"

namespace ezxml
{
    PugiElementIterImpl::PugiElementIterImpl()
            : myIter(), myIterParent(), mySkipProcessingInstructions{ true }, myXDoc()
    {

    }


    PugiElementIterImpl::PugiElementIterImpl(
            const pugi::xml_node_iterator& iter,
            const pugi::xml_node& iterParent,
            const XDocCPtr& parentDoc
    )
            : myIter( iter ), myIterParent( iterParent ), mySkipProcessingInstructions{ true }, myXDoc( parentDoc )
    {

    }


    XElementIterImplUP
    PugiElementIterImpl::clone() const
    {
        return XElementIterImplUP{ new PugiElementIterImpl{ *this } };
    }


    bool
    PugiElementIterImpl::getIsPayloadNull() const
    {
        auto ptr = myXDoc.lock();

        if( !ptr )
        {
            return true;
        }
        else if( myIterParent.type() != pugi::node_element )
        {
            return true;
        }

        return false;
    }


    bool
    PugiElementIterImpl::getIsEndIter() const
    {
        if( myIter == myIterParent.end() )
        {
            return true;
        }

        return false;
    }


    bool
    PugiElementIterImpl::getIsProcessingInstruction() const
    {
        if( this->getIsEndIter() )
        {
            return false;
        }
        else if( this->getIsPayloadNull() )
        {
            return false;
        }

        const auto type = myIter->type();

        if( type == pugi::node_pi )
        {
            return true;
        }

        return false;
    }


    bool
    PugiElementIterImpl::getSkipProcessingInstructions() const
    {
        return mySkipProcessingInstructions;
    }


    void
    PugiElementIterImpl::setSkipProcessingInstructions( bool inValue )
    {
        mySkipProcessingInstructions = inValue;
    }


    bool
    PugiElementIterImpl::equals( const XElementIterator& other ) const
    {

        auto& otherXImplPtr = other.reveal();

        if( !otherXImplPtr )
        {
            return false;
        }

        auto otherPtr = dynamic_cast<PugiElementIterImpl*>( otherXImplPtr.get() );

        return myIter == otherPtr->myIter;
    }


    XElement&
    PugiElementIterImpl::getRef() const
    {
        if( getIsPayloadNull() )
        {
            EZXML_THROW_XNULL;
        }

        if( getIsEndIter() )
        {
            EZXML_THROW( "XElementIterator attempted to dereference an 'end' iterator" );
        }

        myReturnableElement = PugiElement{ *myIter, myXDoc.lock() };
        return myReturnableElement;
    }


    XElement*
    PugiElementIterImpl::getPtr() const
    {
        if( getIsPayloadNull() )
        {
            EZXML_THROW_XNULL;
        }

        if( getIsEndIter() )
        {
            EZXML_THROW( "XElementIterator attempted to dereference an 'end' iterator" );
        }

        myReturnableElement = PugiElement{ *myIter, myXDoc.lock() };
        return &myReturnableElement;
    }


    const PugiElementIterImpl&
    PugiElementIterImpl::increment()
    {
        ++myIter;
        return *this;
    }


    const PugiElementIterImpl&
    PugiElementIterImpl::decrement()
    {
        --myIter;
        return *this;
    }


    pugi::xml_node_type
    PugiElementIterImpl::getPugiXmlNodeType() const
    {
        if( getIsPayloadNull() )
        {
            return pugi::node_null;
        }
        return myIter->type();
    }


    bool
    PugiElementIterImpl::hasTextData() const
    {
        if( getIsPayloadNull() )
        {
            return false;
        }
        return std::string{ myIter->child_value() }.size() > 0;
    }
}
