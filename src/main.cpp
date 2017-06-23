#include "tbin/Map.hpp"

#include <iostream>
#include <util/Logger.hpp>

int main()
{
    util::Logger::setName( "Main log", "log.txt" );

    tbin::Map map;
    if ( !map.loadFromFile( "G:\\StardewValley\\StardewValley-decompiled\\Content-Unpacked\\Maps\\Backwoods.tbin" ) )
    //if ( !map.loadFromFile( "G:\\StardewValley\\Backwoods.tbin" ) )
    {
        return 1;
    }

    if ( !map.saveToFile( "G:\\StardewValley\\Backwoods2.tbin" ) )
        return 2;

    if ( !map.loadFromFile( "G:\\StardewValley\\Backwoods2.tbin" ) )
        return 3;

    return 0;
}
