#include "tbin/Map.hpp"

#include <iostream>
#include <emscripten/emscripten.h>
#include <cstring>

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
        if (!m) return "";
        return m->id.c_str();
    }

    EMSCRIPTEN_KEEPALIVE
    int map_prop_count(const tbin::Map* m) {
        if (!m) return 0;
        return m->props.size();
    }
    
    EMSCRIPTEN_KEEPALIVE
    char** map_props_keys_list(const tbin::Map* m, int* outCount) {
        if (!outCount) return nullptr;
        *outCount = 0;
        if (!m) return nullptr;

        const int n = (int)m->props.size();
        *outCount = n;

        // Allocate the array of pointers
        char** list = (char**)std::malloc(sizeof(char*) * n);
        int idx = 0;

        for (auto const& [k, v] : m->props) {
            // Allocate and copy each key string
            const size_t len = k.size();
            char* s = (char*)std::malloc(len + 1);
            std::memcpy(s, k.c_str(), len + 1);
            list[idx++] = s;
        }

        return list;
    }
    
    EMSCRIPTEN_KEEPALIVE
    void map_free_string_list(char** list, int count) {
        if (!list) return;
        for (int i = 0; i < count; i++) {
            std::free(list[i]);
        }
        std::free(list);
    }
    
    EMSCRIPTEN_KEEPALIVE
    const char* map_prop_get_value(const tbin::Map* m, const char* key) {
        printf("getting value for %s\n", key);
        if (!m) {
            printf("m is null getting prop value for %s", key);
            return "";
        }
        auto it = m->props.find(key);
        if (it == m->props.end()) {
            printf("couldnt find %s in properties\n", key);
            return nullptr;
        }

        const tbin::PropertyValue* pv = &it->second;   // assigned to a variable
        const char* val = pv->dataStr.c_str();
        printf("[c] key %s equals value %s\n", key, val);
        return val;
    }

    EMSCRIPTEN_KEEPALIVE
    int map_tilesheets_count(const tbin::Map* m) {
        if (!m) return 0;

        return m->tilesheets.size();
    }
    
    EMSCRIPTEN_KEEPALIVE
    const char* map_tilesheets_get_name(const tbin::Map* m, int index) {
        if (!m) return 0;

        return m->tilesheets.at(index).id.c_str();
    }
    
    EMSCRIPTEN_KEEPALIVE
    const char* map_tilesheets_get_image(const tbin::Map* m, int index) {
        if (!m) return 0;

        return m->tilesheets.at(index).image.c_str();
    }
    
    EMSCRIPTEN_KEEPALIVE
    int map_tilesheets_prop_count(const tbin::Map* m, int index) {
        if (!m) return 0;

        return m->tilesheets.at(index).props.size();
    }

    EMSCRIPTEN_KEEPALIVE
    char** map_tilesheet_prop_keys_list(const tbin::Map* m, int* outCount, int index) {
        if (!outCount) return nullptr;
        *outCount = 0;
        if (!m) return nullptr;

        tbin::TileSheet sheet = m->tilesheets.at(index);

        const int n = (int)sheet.props.size();
        *outCount = n;

        // Allocate the array of pointers
        char** list = (char**)std::malloc(sizeof(char*) * n);
        int idx = 0;

        for (auto const& [k, v] : sheet.props) {
            // Allocate and copy each key string
            const size_t len = k.size();
            char* s = (char*)std::malloc(len + 1);
            std::memcpy(s, k.c_str(), len + 1);
            list[idx++] = s;
        }

        return list;
    }
    
    EMSCRIPTEN_KEEPALIVE
    const char* map_tilesheet_prop_get_value(const tbin::Map* m, const char* key, int index) {
        printf("tilesheet getting value for %s\n", key);
        if (!m) {
            printf("tilesheet m is null getting prop value for %s", key);
            return "";
        }
        tbin::TileSheet sheet = m->tilesheets.at(index);
        
        auto it = m->props.find(key);
        if (it == m->props.end()) {
            printf("tilesheet couldnt find %s in properties\n", key);
            return nullptr;
        }

        const tbin::PropertyValue* pv = &it->second;   // assigned to a variable
        const char* val = pv->dataStr.c_str();
        printf("tilesheet [c] key %s equals value %s\n", key, val);
        return val;
    }

    EMSCRIPTEN_KEEPALIVE
    int map_tilesheets_tile_x(const tbin::Map* m, int index) {
        if (!m) return 0;

        return m->tilesheets.at(index).tileSize.x;
    }
    
    EMSCRIPTEN_KEEPALIVE
    int map_tilesheets_tile_y(const tbin::Map* m, int index) {
        if (!m) return 0;

        return m->tilesheets.at(index).tileSize.y;
    }
    
    EMSCRIPTEN_KEEPALIVE
    int map_tilesheets_image_px_x(const tbin::Map* m, int index) {
        if (!m) return 0;

        return m->tilesheets.at(index).sheetSize.x;
    }
    
    EMSCRIPTEN_KEEPALIVE
    int map_tilesheets_image_px_y(const tbin::Map* m, int index) {
        if (!m) return 0;

        return m->tilesheets.at(index).sheetSize.y;
    }
    
    EMSCRIPTEN_KEEPALIVE
    int map_layers_count(const tbin::Map* m) {
        if (!m) return 0;

        return m->layers.size();
    }

    EMSCRIPTEN_KEEPALIVE
    const char* map_layer_get_name(const tbin::Map* m, int index) {
        if (!m) return 0;

        return m->layers.at(index).id.c_str();
    }
    
    EMSCRIPTEN_KEEPALIVE
    int map_layer_get_tile_width(const tbin::Map* m, int index) {
        if (!m) return 0;

        return m->layers.at(index).layerSize.x;
    }
    
    EMSCRIPTEN_KEEPALIVE
    int map_layer_get_tile_height(const tbin::Map* m, int index) {
        if (!m) return 0;

        return m->layers.at(index).layerSize.y;
    }
    
    EMSCRIPTEN_KEEPALIVE
    int map_layer_get_visiblity(const tbin::Map* m, int index) {
        if (!m) return 0;

        return m->layers.at(index).visible;
    }
    
    EMSCRIPTEN_KEEPALIVE
    int map_layer_get_tile_width_px_x(const tbin::Map* m, int index) {
        if (!m) return 0;

        return m->layers.at(index).tileSize.x;
    }
    
    EMSCRIPTEN_KEEPALIVE
    int map_layer_get_tile_width_px_y(const tbin::Map* m, int index) {
        if (!m) return 0;

        return m->layers.at(index).tileSize.y;
    }

}