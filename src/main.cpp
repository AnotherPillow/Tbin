#include "tbin/Map.hpp"

#include <iostream>

int main(int argv, char** argc)
{
    if (3 > argv) {
        printf("please run with [executable] input.tbin output.tbin\n");
        return 1;
    }

    tbin::Map map;
    if ( !map.loadFromFile(argc[1]) )
    {
        return 1;
    }

    std::map< std::string, int > tss;
    for ( auto ts : map.tilesheets )
    {
        printf("%s %s %d,%d %d,%d\n", ts.id.c_str(), ts.image.c_str(), ts.sheetSize.x, ts.sheetSize.y, ts.tileSize.x, ts.tileSize.y );
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

    if ( !map.saveToFile( argc[2] ) )
        return 2;

    if ( !map.loadFromFile( argc[2] ) )
        return 3;

    return 0;
}
