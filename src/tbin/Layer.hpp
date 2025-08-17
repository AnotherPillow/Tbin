#ifndef TBIN_LAYER_HPP
#define TBIN_LAYER_HPP

//#include <SFML/System/Vector2.hpp>
#include <string>
#include <vector>

#include "tbin/PropertyValue.hpp"
#include "tbin/Tile.hpp"
#include "tbin/Vector2.hpp"

namespace tbin
{
    struct Layer
    {
        public:
            std::string id;
            bool visible;
            std::string desc;
            Vector2 layerSize;
            Vector2 tileSize;
            Properties props;
            std::vector< Tile > tiles;
    };
}

#endif // TBIN_LAYER_HPP
