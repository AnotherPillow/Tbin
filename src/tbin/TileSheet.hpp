#ifndef TBIN_TILESHEET_HPP
#define TBIN_TILESHEET_HPP

#include <string>

#include "tbin/PropertyValue.hpp"
#include "tbin/Vector2.hpp"

namespace tbin
{
    struct TileSheet
    {
        public:
            std::string id;
            std::string desc;
            std::string image;
            Vector2 sheetSize;
            Vector2 tileSize;
            Vector2 margin;
            Vector2 spacing;
            Properties props;
    };
}

#endif // TBIN_TILESHEET_HPP
