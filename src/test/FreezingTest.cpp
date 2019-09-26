#include <sstream>

#include "Catch.h"
#include "ezxml/ezxml.h"
#include "Files.h"

TEST_CASE( "FreezingTest" )
{
    using namespace ezxml;
    using namespace ezxmltest;
    const auto fp = filepath( FREEZING_SMALL );
    auto xdocOriginal = XFactory::makeXDoc();
    xdocOriginal->loadFile( fp );
    auto xdoc = std::move( xdocOriginal );

    const auto& root = xdoc->getRoot();
    auto str = root->getName();
    CHECK( "score-partwise" == str );
    auto aiter = root->attributesBegin();
    auto aend = root->attributesEnd();
    REQUIRE( aiter != aend );
    str = aiter->getName();
    CHECK( "version" == str );
    str = aiter->getValue();
    CHECK( "3.0" == str );
    ++aiter;
    CHECK( aend == aiter );

    auto iter = root->begin();

    std::stringstream saveStream;
    xdoc->saveStream( saveStream );
}