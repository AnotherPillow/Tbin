#include "tbin/Map.hpp"

#include <iostream>
#include <emscripten/emscripten.h>

int main(int argv, char** argc)
{
    if (3 > argv) {
        printf("please run with [executable] input.tbin output.tbin");
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

extern "C" {

    EMSCRIPTEN_KEEPALIVE
    tbin::Map* map_create() {
        return new tbin::Map();
    }

    EMSCRIPTEN_KEEPALIVE
    void map_destroy(tbin::Map* m) {
        delete m;
    }

    EMSCRIPTEN_KEEPALIVE
    int map_load_file(tbin::Map* m, const char* vfsPath) {
        return (m && m->loadFromFile(vfsPath)) ? 1 : 0;
    }

    // Example getters (you implement what you need)
    EMSCRIPTEN_KEEPALIVE
    void map_log_props(const tbin::Map* m) {
        if (!m) return;

        for (auto const& [key, val] : m->props) {
            printf("%s -> %s\n", key.c_str(), val.dataStr.c_str());
        }
        return;
    }

    EMSCRIPTEN_KEEPALIVE
    const char* map_get_id(const tbin::Map* m) {
        if (!m) { return ""; };
        return m->id.c_str();
    }

}