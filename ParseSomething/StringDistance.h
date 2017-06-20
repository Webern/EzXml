
#pragma once

#include <string>
#include <vector>
#include <algorithm>

namespace x
{
    inline std::vector<std::string> getBigrams( const std::string& inString )
    {
        std::vector<std::string> bigrams;
        auto char1 = inString.cbegin();
        auto char2 = char1 + 1;
        const auto e = inString.cend();
        for( ; char1 != e && char2 != e; ++char1, ++char2 )
        {
            std::string bigram;
            std::copy( char1, char2 + 1, std::back_inserter(bigram) );
            bigrams.emplace_back( std::move( bigram ) );
        }

        return bigrams;
    }

    // 1 means completely
    inline double getDistance( const std::string& inLeft, const std::string& inRight, bool isCaseSensitive = false )
    {
        std::string left = inLeft;
        std::string right = inRight;

        if( !isCaseSensitive )
        {
            std::transform(left.begin(), left.end(), left.begin(), ::toupper);
            std::transform(right.begin(), right.end(), right.begin(), ::toupper);
        }

        auto lbigrams = getBigrams( left );
        auto rbigrams = getBigrams( right );
        std::sort( std::begin(lbigrams), std::end(lbigrams) );
        std::sort( std::begin(rbigrams), std::end(rbigrams) );

        std::vector<std::string> intersection;
        std::set_intersection( std::begin( lbigrams ), std::end( lbigrams ), std::begin( rbigrams ), std::end( rbigrams ), std::inserter(intersection, intersection.begin() ) );

        double numerator = static_cast<double>( intersection.size() ) * 2.0;
        double denominator = static_cast<double>( lbigrams.size() ) + static_cast<double>( rbigrams.size() );

        if( denominator == 0.0 )
        {
            return 0;
        }
        const double closeness = numerator / denominator;
        const double distance = 1 - closeness;
        return distance;
    }
}
