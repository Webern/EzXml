#include <sstream>

#include "Catch.h"
#include "ezxml/ezxml.h"
#include "Files.h"

namespace ezxmltest
{
    inline void
    recurse( const ezxml::XElement& current, int currentElementLikeID )
    {
        auto iter = current.beginWithProcessingInstructions();
//        iter.setSkipProcessingInstructions( false );
        if( current.getIsProcessingInstruction() )
        {
            std::cout << "node: " << currentElementLikeID << ", " << current.getName();
        }
        const auto xtype = current.getType();
        if( xtype == ezxml::XElementType::element )
        {
            int childElementLikeID = currentElementLikeID + 1;
            const auto end = current.end();
            for( ; iter != end; ++iter, ++childElementLikeID )
            {
                recurse( *iter, childElementLikeID );
            }
        }
    }
}

TEST_CASE( "ProcessingInstructions" )
{
    using namespace ezxml;
    using namespace ezxmltest;
    const auto fp = filepath( FREEZING_SMALL );
    const auto xdoc = XFactory::makeXDoc();
    xdoc->loadFile( fp );
    const auto root = xdoc->getRoot();
    ezxmltest::recurse( *root, 0 );
}