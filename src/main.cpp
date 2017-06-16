#include "tbin/Map.hpp"

#include <iostream>
#include <util/Logger.hpp>

int main()
{
    util::Logger::setName( "Main log", "log.txt" );

    tbin::Map map;
    if ( !map.loadFromFile( "G:\\StardewValley\\StardewValley-decompiled\\Content-Unpacked\\Maps\\Tent.tbin" ) )
    {
        return 1;
    }

    if ( !map.saveToFile( "out.tbin" ) )
        return 2;

    return 0;
}
