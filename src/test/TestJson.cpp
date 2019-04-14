#include <sstream>

#include "Catch.h"
#include "ezxml/ezxml.h"

static constexpr const char* const xml = R"(
<whatever abc="def" ghi="jkl">
    <bloop>blerp</bloop>
    <bloop>bleep</bloop>
</whatever>
)";

static constexpr const char* const json = R"(
{
    "attr_abc": "def",
    "attr_ghi": "jkl",
    "bloop_array": [
        "blerp",
        "bleep"
    ]
}
)";

TEST_CASE( "basic_json", "1" )
{
    auto jdoc = ezxml::XFactory::makeJDoc( "whatever", "attr_", "bloop" );
    std::istringstream iss;
    jdoc->loadStream( iss );
}