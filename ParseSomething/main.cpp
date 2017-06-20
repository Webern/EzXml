//
//  main.cpp
//  ParseSomething
//
//  Created by mjb on 6/12/17.
//  Copyright © 2017 Matthew James BRiggs. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include "ezxml/ezxml.h"
#include "StringDistance.h"

const char* const inMusicXmlSoundsFilepath = "/Users/mjb/Dropbox/Programming/MxRepo/Documents/DevNotes/MusicXMLSounds.xml";
const char* const inFilepathMidiRef = "/Users/mjb/komp/Resources/Documents/DevNotes/GeneralMidiReference.xml";
const char* const inFilepathGmPrioritySort = "/Users/mjb/komp/Resources/Documents/DevNotes/generalMidiPrioritySort.xml";

struct GmToInstrument
{
    int gm;
    std::string gmn;
    bool isGoodMatch;
    std::string bestMatch;
    std::vector<std::string> otherMatches;

    GmToInstrument()
    : gm(-1)
    , gmn("")
    , isGoodMatch(true)
    , bestMatch("")
    , otherMatches()
    {

    }

};

struct sound
{
    std::string id;
    std::vector<std::string> path;
};

int main(int argc, const char * argv[]) {
    using namespace ezxml;
    auto xdoc = XFactory::makeXDoc();
    xdoc->loadFile( inMusicXmlSoundsFilepath );

    auto xdocGmRef = XFactory::makeXDoc();
    xdocGmRef->loadFile( inFilepathMidiRef );

    std::map<std::string, int> gms;
    const auto gmXdocRoot = xdocGmRef->getRoot();
    for( auto it = gmXdocRoot->begin(); it != gmXdocRoot->end(); ++it )
    {
        const auto gmElem = it->begin();
        const auto gmnElem = ( ++( it->begin() ) );
        gms.emplace( gmnElem->getValue(), std::stoi( gmElem->getValue() ) );
    }

    std::vector<sound> sounds;
    for( auto it = xdoc->getRoot()->begin(); it != xdoc->getRoot()->end(); ++it )
    {
        sound s;
        s.id = it->getValue();
        std::stringstream bldr;
        for( auto rit = s.id.crbegin(); rit != s.id.crend(); ++rit )
        {
            if( *rit != '.' )
            {
                bldr << *rit;
            }
            else
            {
                const auto backwards = bldr.str();
                bldr.str( "" );
                std::stringstream forwards;
                for( auto c = backwards.crbegin(); c != backwards.crend(); ++c )
                {
                    forwards << *c;
                }
                s.path.push_back( forwards.str() );
            }
        }
        const auto backwards = bldr.str();
        bldr.str( "" );
        std::stringstream forwards;
        for( auto c = backwards.crbegin(); c != backwards.crend(); ++c )
        {
            forwards << *c;
        }
        s.path.push_back( forwards.str() );
        sounds.emplace_back( s );
    }

    std::vector<GmToInstrument> instruments;
    for( const auto& pair : gms )
    {
        GmToInstrument i;
        i.gm = pair.second;
        i.gmn = pair.first;
        instruments.emplace_back( i );
    }

    auto sortByDistance = [&]( const std::string inString, const std::vector<sound>& inSounds ) {
        std::vector<std::pair<double, sound>> distanceVector;
        for( const auto& sound : inSounds )
        {
            const auto distance = x::getDistance( inString, sound.id );
            distanceVector.emplace_back( distance, sound );
        }

        std::sort(std::begin(distanceVector), std::end(distanceVector), [&](const std::pair<double, sound>& l, const std::pair<double, sound>& r)
                  {
                      return l.first < r.first;
                  });
        std::vector<sound> result;
        for( const auto& pair : distanceVector )
        {
            result.push_back( pair.second );
        }
        return result;
    };

    auto findSound = [&]( const std::string name )
    {
        sounds = sortByDistance( name, sounds );
        return sounds.front();
    };



    for( const auto& gmPair : gms )
    {
        auto iter = std::find_if(instruments.begin(), instruments.end(), [&](const GmToInstrument& inInstrument){ return inInstrument.gm == gmPair.second; } );

        if( gmPair.first == "Alto Sax" )
        {
            iter->bestMatch = "wind.reed.saxophone.alto";
        }
        else if( gmPair.first == "Baritone Sax" )
        {
            iter->bestMatch = "wind.reed.saxophone.baritone";
        }
        else if( gmPair.first == "Brass Section" )
        {
            iter->bestMatch = "brass.group";
        }
        else if( gmPair.first == "Bright Acoustic Piano" )
        {
            iter->bestMatch = "keyboard.piano";
            iter->isGoodMatch = false;
        }
        else if( gmPair.first == "Choir Aahs" )
        {
            iter->bestMatch = "voice.aa";
        }
        else if( gmPair.first == "Church Organ" )
        {
            iter->bestMatch = "keyboard.organ.pipe";
        }
        else if( gmPair.first == "Clavi" )
        {
            iter->bestMatch = "keyboard.clavichord";
        }
        else if( gmPair.first == "Distortion Guitar" )
        {
            iter->bestMatch = "pluck.guitar.electric";
            iter->isGoodMatch = false;
        }
        else if( gmPair.first == "Electric Piano 2" )
        {
            iter->bestMatch = "keyboard.piano.electric";
            iter->isGoodMatch = false;
        }
        else if( gmPair.first == "Flute" )
        {
            iter->bestMatch = "wind.flutes.flute";
        }
        else if( gmPair.first == "Guitar harmonics" )
        {
            iter->bestMatch = "pluck.guitar.acoustic";
            iter->isGoodMatch = false;
        }
        else if( gmPair.first == "Kalimba" )
        {
            iter->bestMatch = "pitched-percussion.kalimba";
        }
        else if( gmPair.first == "Lead 5 (charang)" )
        {
            iter->bestMatch = "pluck.synth.charang";
        }
        else if( gmPair.first == "Lead 6 (voice)" )
        {
            iter->bestMatch = "voice.synth";
            iter->isGoodMatch = false;
        }
        else if( gmPair.first == "Marimba" )
        {
            iter->bestMatch = "pitched-percussion.marimba";
        }
        else if( gmPair.first == "Melodic Tom" )
        {
            iter->bestMatch = "drum.rototom";
        }
        else if( gmPair.first == "Music Box" )
        {
            iter->bestMatch = "pitched-percussion.music-box";
        }
        else if( gmPair.first == "Muted Trumpet" )
        {
            iter->bestMatch = "brass.trumpet";
            iter->isGoodMatch = false;
        }
        else if( gmPair.first == "Orchestral Harp" )
        {
            iter->bestMatch = "pluck.harp";
        }
        else if( gmPair.first == "Overdriven Guitar" )
        {
            iter->bestMatch = "pluck.guitar.electric";
            iter->isGoodMatch = false;
        }
        else if( gmPair.first == "Piccolo" )
        {
            iter->bestMatch = "wind.flutes.flute.piccolo";
        }
        else if( gmPair.first == "Pizzicato Strings" )
        {
            iter->bestMatch = "strings.group";
            iter->isGoodMatch = false;
        }
        else if( gmPair.first == "Rock Organ" )
        {
            iter->bestMatch = "keyboard.organ.rotary";
        }
        else if( gmPair.first == "Soprano Sax" )
        {
            iter->bestMatch = "wind.reed.saxophone.soprano";
        }
        else if( gmPair.first == "String Ensemble 1" )
        {
            iter->bestMatch = "strings.group";
        }
        else if( gmPair.first == "String Ensemble 2" )
        {
            iter->bestMatch = "strings.group";
            iter->isGoodMatch = false;
        }
        else if( gmPair.first == "Tenor Sax" )
        {
            iter->bestMatch = "voice.tenor";
        }
        else if( gmPair.first == "Tremolo Strings" )
        {
            iter->bestMatch = "strings.group";
            iter->isGoodMatch = false;
        }
        else if( gmPair.first == "Vibraphone" )
        {
            iter->bestMatch = "pitched-percussion.vibraphone";
        }
        else if( gmPair.first == "Xylophone" )
        {
            iter->bestMatch = "pitched-percussion.xylophone";
        }
        else if( gmPair.first == "Agogo" )
        {
            iter->bestMatch = "metal.bells.agogo";
        }
        else if( gmPair.first == "Acoustic Grand Piano" )
        {
            iter->bestMatch = "keyboard.piano";
        }
        else if( gmPair.first == "Acoustic Guitar (steel)" )
        {
            iter->bestMatch = "pluck.guitar.steel-string";
        }
        else if( gmPair.first == "Electric Guitar (clean)" )
        {
            iter->bestMatch = "pluck.guitar.electric";
            iter->isGoodMatch = true;
        }
        else if( gmPair.first == "Electric Bass (pick)" )
        {
            iter->bestMatch = "pluck.bass.electric";
            iter->isGoodMatch = false;
        }
        else if( gmPair.first == "Synth Bass 2" )
        {
            iter->bestMatch = "pluck.bass.synth";
            iter->isGoodMatch = false;
        }
        else if( gmPair.first == "SynthBrass 2" )
        {
            iter->bestMatch = "brass.group.synth";
            iter->isGoodMatch = false;
        }
        else if( gmPair.first == "SynthStrings 2" )
        {
            iter->bestMatch = "strings.group.synth";
            iter->isGoodMatch = false;
        }
        else if( gmPair.first == "Tango Accordion" )
        {
            iter->bestMatch = "keyboard.accordion";
            iter->isGoodMatch = false;
        }
        else if( gmPair.first == "Tenor Sax" )
        {
            iter->bestMatch = "wind.reed.saxophone.tenor";
        }
        else if( gmPair.first == "" )
        {
            iter->bestMatch = "";
        }
        else if( gmPair.first == "" )
        {
            iter->bestMatch = "";
        }
        else if( gmPair.first == "" )
        {
            iter->bestMatch = "";
        }
        else if( gmPair.first == "" )
        {
            iter->bestMatch = "";
        }
        else if( gmPair.first == "" )
        {
            iter->bestMatch = "";
        }
        else if( gmPair.first == "" )
        {
            iter->bestMatch = "";
        }
        else if( gmPair.first == "" )
        {
            iter->bestMatch = "";
        }
        else if( gmPair.first == "" )
        {
            iter->bestMatch = "";
        }
        else if( gmPair.first == "" )
        {
            iter->bestMatch = "";
        }

        else
        {
            iter->bestMatch = findSound( gmPair.first ).id;
        }
        std::cout << "gm: " << gmPair.first << ", mx: " << iter->bestMatch << std::endl;
    }

    auto soundsCopy = sounds;

    for ( auto& instrument : instruments )
    {
        const auto iter = std::find_if( std::begin( soundsCopy ), std::end( soundsCopy ), [&]( const sound& inSound ){ return inSound.id == instrument.bestMatch; } );
        if( iter != std::end( soundsCopy ) )
        {
            soundsCopy.erase( iter );
        }
    }

    auto sortByDistance2 = [&]( const std::string inString, const std::vector<GmToInstrument>& inInstruments ) {
        std::vector<std::pair<double, GmToInstrument>> distanceVector;
        for( const auto& instr : inInstruments )
        {
            const auto distance1 = x::getDistance( inString, instr.bestMatch );
            const auto distance2 = x::getDistance( inString, instr.gmn );
            distanceVector.emplace_back( distance1, instr );
        }

        std::sort(std::begin(distanceVector), std::end(distanceVector), [&](const std::pair<double, GmToInstrument>& l, const std::pair<double, GmToInstrument>& r)
                  {
                      return l.first < r.first;
                  });
        std::vector<GmToInstrument> result;
        for( const auto& pair : distanceVector )
        {
            result.push_back( pair.second );
        }
        return result;
    };

    while( soundsCopy.cbegin() != soundsCopy.cend() )
    {
        const auto iter = soundsCopy.cbegin();

        if( iter == soundsCopy.cend() )
        {
            break;
        }

        const auto currentSoundId = iter->id;

        std::vector<std::string> searchables;
        for( const auto& instrument : instruments )
        {
            searchables.push_back( instrument.bestMatch );
        }
        const auto sortedBestMatches = sortByDistance2( currentSoundId, instruments );
        auto best = sortedBestMatches.cbegin();
        while( !best->isGoodMatch )
        {
            ++best;
        }
        const auto findit = std::find_if( instruments.begin(), instruments.end(), [&]( const GmToInstrument& inInstrument) { return inInstrument.bestMatch == best->bestMatch; } );
        findit->otherMatches.push_back( currentSoundId );
        soundsCopy.erase( iter );
    }
    std::sort( std::begin( instruments ), std::end( instruments ), [&]( const GmToInstrument& l, const GmToInstrument& r ) { return l.gm < r.gm; } );

    auto outx = XFactory::makeXDoc();
    auto root = outx->getRoot();
    root->setName( "instruments" );
    auto gmtoid = root->appendChild( "gm-to-instrument-id" );
    for( const auto& instrument : instruments )
    {
        auto child = gmtoid->appendChild( "gm-instrument" );
        auto gm = child->appendChild( "gm" );
        gm->setValue( std::to_string( instrument.gm ) );

        auto gmn = child->appendChild( "gmn" );
        gmn->setValue( instrument.gmn );

        auto bestMatch = child->appendChild( "best-match" );
        bestMatch->setValue( instrument.bestMatch );

        auto isGoodMatchElement = child->appendChild( "is-good-match" );
        std::stringstream isgood;
        isgood << std::boolalpha << instrument.isGoodMatch;
        isGoodMatchElement->setValue( isgood.str() );

        auto additionalMatchesElement = child->appendChild( "additional-matches" );

        for( const auto& additionalMatch : instrument.otherMatches )
        {
            auto additionalMatchElement = additionalMatchesElement->appendChild( "additional-match" );
            additionalMatchElement->setValue( additionalMatch );
        }
    }

    outx->saveFile( "/Users/mjb/Dropbox/Programming/MxRepo/Documents/DevNotes/GmMapSoundsTemp.xml" );

    return 0;
}
