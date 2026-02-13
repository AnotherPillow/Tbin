#ifndef TBIN_TILE_HPP
#define TBIN_TILE_HPP

#include <vector>
#include <cstdint>

#include "tbin/PropertyValue.hpp"

namespace tbin
{
    struct Tile
    {
        public:
            enum Type
            {
                Null,
                Static,
                Animated,
            };
            
            std::string tilesheet;
            
            struct
            {
                std::int32_t tileIndex;
                std::uint8_t blendMode;
            } staticData;
            
            struct
            {
                std::int32_t frameInterval;
                std::vector< Tile > frames;
            } animatedData;
            
            Properties props;
            
            inline bool isNullTile() const { return staticData.tileIndex == -1 && animatedData.frames.size() == 0; }
    };
}

#endif // TBIN_TILE_HPP
                    
