#include "tbin/Map.hpp"

#include <iostream>
#include <util/Logger.hpp>

int main()
{
    util::Logger::setName( "Main log", "log.txt" );

    tbin::Map map;
    //if ( !map.loadFromFile( "G:\\StardewValley\\StardewValley-decompiled\\Content-Unpacked\\Maps\\Backwoods.tbin" ) )
    //if ( !map.loadFromFile( "G:\\StardewValley\\StardewValley-decompiled\\Content-Unpacked\\Maps\\BathHouse_Entry.tbin" ) )
    if ( !map.loadFromFile( "G:\\StardewValley\\Backwoods.tbin" ) )
    {
        return 1;
    }

    std::map< std::string, int > tss;
    for ( auto ts : map.tilesheets )
    {
        util::log("$ $ $,$ $,$\n", ts.id, ts.image, ts.sheetSize.x, ts.sheetSize.y, ts.tileSize.x, ts.tileSize.y );
        tss.insert( std::make_pair( ts.id, ts.sheetSize.x * ts.sheetSize.y ) );
    }

    for ( auto layer : map.layers )
    {
        for ( std::size_t i = 0; i < layer.tiles.size(); ++i )
        {
            auto tile = layer.tiles[ i ];
            if ( tile.isNullTile() ) continue;
            if ( tile.staticData.tileIndex >= tss[ tile.tilesheet ] && tile.animatedData.frames.size() == 0 )
            {
                std::cout << "Bad tile index: " << layer.id << " '" << tile.tilesheet << "' " << (i%layer.layerSize.x)<<','<< (i/layer.layerSize.x) << ' ' << tile.staticData.tileIndex << ' ' << tss[ tile.tilesheet ] << std::endl;
            }
        }
    }

    if ( !map.saveToFile( "G:\\StardewValley\\Backwoods2.tbin" ) )
        return 2;

    if ( !map.loadFromFile( "G:\\StardewValley\\Backwoods2.tbin" ) )
        return 3;

    return 0;
}
