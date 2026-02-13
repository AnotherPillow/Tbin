#include "tbin/Map.hpp"

#include <fstream>
#include <stdexcept>
#include <cstdint>
//#include <util/Logger.hpp>

namespace tbin
{
    template< typename T >
    T read( std::istream& in )
    {
        T t;
        in.read( reinterpret_cast< char* >( &t ), sizeof( T ) );
        return t;
    }

    template<>
    Vector2 read< Vector2 >( std::istream& in )
    {
        std::int32_t x = read< std::int32_t >( in );
        std::int32_t y = read< std::int32_t >( in );
        return Vector2( x, y );
    }

    template<>
    std::string read< std::string >( std::istream& in )
    {
        auto len = read< std::int32_t >( in );
        std::string str( len, 0 );
        in.read( &str[ 0 ], len );
        return str;
    }

    template< typename T >
    void write( std::ostream& out, const T& t )
    {
        out.write( reinterpret_cast< const char* >( &t ), sizeof( T ) );
    }

    template<>
    void write< Vector2 >( std::ostream& out, const Vector2& vec )
    {
        write< std::int32_t >( out, vec.x );
        write< std::int32_t >( out, vec.y );
    }

    template<>
    void write< std::string >( std::ostream& out, const std::string& str )
    {
        write< std::int32_t >( out, str.length() );
        out.write( &str[ 0 ], str.length() );
    }

    Properties readProperties( std::istream& in )
    {
        Properties ret;

        int count = read< std::int32_t >( in );
        for ( int i = 0; i < count; ++i )
        {
            std::string key;
            PropertyValue value;

            key = read< std::string >( in );
            value.type = static_cast< PropertyValue::Type >( read< std::uint8_t >( in ) );
            switch ( value.type )
            {
                case PropertyValue::Bool:    value.data.b  = read< std::uint8_t   >( in ) > 0; break;
                case PropertyValue::Integer: value.data.i  = read< std::int32_t   >( in );     break;
                case PropertyValue::Float:   value.data.f  = read< float       >( in );     break;
                case PropertyValue::String:  value.dataStr = read< std::string >( in );     break;
                default: throw std::invalid_argument("Bad property type");
            }

            ret[ key ] = value;
        }


        return ret;
    }

    void writeProperties( std::ostream& out, const Properties& props )
    {
        write< std::int32_t >( out, props.size() );
        for ( const auto& prop : props )
        {
            write( out, prop.first );
            write< std::uint8_t >( out, prop.second.type );
            switch ( prop.second.type )
            {
                case PropertyValue::Bool: write< std::uint8_t >( out, prop.second.data.b ? 1 : 0 ); break;
                case PropertyValue::Integer: write( out, prop.second.data.i ); break;
                case PropertyValue::Float: write( out, prop.second.data.f ); break;
                case PropertyValue::String: write( out, prop.second.dataStr ); break;
                default: throw std::invalid_argument("Bad property type");
            }
        }
    }

    TileSheet readTilesheet( std::istream& in )
    {
        TileSheet ret;
        ret.id = read< std::string >( in );
        ret.desc = read< std::string >( in );
        ret.image = read< std::string >( in );
        ret.sheetSize = read< Vector2 >( in );
        ret.tileSize = read< Vector2 >( in );
        ret.margin = read< Vector2 >( in );
        ret.spacing = read< Vector2 >( in );
        ret.props = readProperties( in );
        return ret;
    }

    void writeTilesheet( std::ostream& out, const TileSheet& ts )
    {
        write( out, ts.id );
        write( out, ts.desc );
        write( out, ts.image );
        write( out, ts.sheetSize );
        write( out, ts.tileSize );
        write( out, ts.margin );
        write( out, ts.spacing );
        writeProperties( out, ts.props );
    }

    Tile readStaticTile( std::istream& in, const std::string& currTilesheet )
    {
        Tile ret;
        ret.tilesheet = currTilesheet;
        ret.staticData.tileIndex = read< std::int32_t >( in );
        ret.staticData.blendMode = read< std::uint8_t >( in );
        ret.props = readProperties( in );
        return ret;
    }

    void writeStaticTile( std::ostream& out, const Tile& tile )
    {
        write( out, tile.staticData.tileIndex );
        write( out, tile.staticData.blendMode );
        writeProperties( out, tile.props );
    }

    Tile readAnimatedTile( std::istream& in )
    {
        Tile ret;
        ret.animatedData.frameInterval = read< std::int32_t >( in );

        int frameCount = read< std::int32_t >( in );
        ret.animatedData.frames.reserve( frameCount );
        std::string currTilesheet;
        for ( int i = 0; i < frameCount; )
        {
            char c = in.get();
            switch ( c )
            {
                case 'T':
                    currTilesheet = read< std::string >( in );
                    break;
                case 'S':
                    ret.animatedData.frames.push_back( readStaticTile( in, currTilesheet ) );
                    ++i;
                    break;
                default:
                    throw std::invalid_argument("Bad layer tile data");
            }
        }

        ret.props = readProperties( in );

        return ret;
    }

    void writeAnimatedTile( std::ostream& out, const Tile& tile )
    {
        write( out, tile.animatedData.frameInterval );
        write< std::int32_t >( out, tile.animatedData.frames.size() );

        std::string currTilesheet;
        for ( const Tile& tile : tile.animatedData.frames )
        {
            if ( tile.tilesheet != currTilesheet )
            {
                write< std::uint8_t >( out, 'T' );
                write( out, tile.tilesheet );
                currTilesheet = tile.tilesheet;
            }

            write< std::uint8_t >( out, 'S' );
            writeStaticTile( out, tile );
        }

        writeProperties( out, tile.props );
    }

    Layer readLayer( std::istream& in )
    {
        Layer ret;
        ret.id = read< std::string >( in );
        ret.visible = read< std::uint8_t >( in ) > 0;
        ret.desc = read< std::string >( in );
        ret.layerSize = read< Vector2 >( in );
        ret.tileSize = read< Vector2 >( in );
        ret.props = readProperties( in );

        Tile nullTile; nullTile.staticData.tileIndex = -1;
        ret.tiles.resize( ret.layerSize.x * ret.layerSize.y, nullTile );

        std::string currTilesheet = "";
        for ( int iy = 0; iy < ret.layerSize.y; ++iy )
        {
            int ix = 0;
            while ( ix < ret.layerSize.x )
            {
                std::uint8_t c = read< std::uint8_t >( in );
                switch ( c )
                {
                    case 'N':
                        ix += read< std::int32_t >( in );
                        break;
                    case 'S':
                        ret.tiles[ ix + iy * ret.layerSize.x ] = readStaticTile( in, currTilesheet );
                        ++ix;
                        break;
                    case 'A':
                        ret.tiles[ ix + iy * ret.layerSize.x ] = readAnimatedTile( in );
                        ++ix;
                        break;
                    case 'T':
                        currTilesheet = read< std::string >( in );
                        break;
                    default:
                        throw std::invalid_argument("Bad layer tile data");
                }
            }
        }

        return ret;
    }

    void writeLayer( std::ostream& out, const Layer& layer )
    {
        write( out, layer.id );
        write< std::uint8_t >( out, layer.visible ? 1 : 0 );
        write( out, layer.desc );
        write( out, layer.layerSize );
        write( out, layer.tileSize );
        writeProperties( out, layer.props );

        std::string currTilesheet = "";
        for ( int iy = 0; iy < layer.layerSize.y; ++iy )
        {
            std::int32_t nulls = 0;
            for ( int ix = 0; ix < layer.layerSize.x; ++ix )
            {
                const Tile& tile = layer.tiles[ ix + iy * layer.layerSize.x ];

                if ( tile.isNullTile() )
                {
                    ++nulls;
                    continue;
                }

                if ( nulls > 0 )
                {
                    write< std::uint8_t >( out, 'N' );
                    write( out, nulls );
                    nulls = 0;
                }

                if ( tile.tilesheet != currTilesheet )
                {
                    write< std::uint8_t >( out, 'T' );
                    write( out, tile.tilesheet );
                    currTilesheet = tile.tilesheet;
                }

                if ( tile.animatedData.frames.size() == 0 )
                {
                    write< std::uint8_t >( out, 'S' );
                    writeStaticTile( out, tile );
                }
                else
                {
                    write< std::uint8_t >( out, 'A' );
                    writeAnimatedTile( out, tile );
                }
            }

            if ( nulls > 0 )
            {
                write< std::uint8_t >( out, 'N' );
                write( out, nulls );
            }
        }
    }

    constexpr const char* MAGIC_1_0 = "tBIN10";

    bool Map::loadFromFile( const std::string& path )
    {
        std::ifstream file( path, std::ifstream::binary );
        if ( !file )
        {
            printf( "[ERROR] %s: Failed to open file.\n", path.c_str() );
            return false;
        }

        printf( "[INFO] Reading %s...\n", path.c_str() );
        return loadFromStream( file );
    }

    bool Map::loadFromStream( std::istream& in )
    {
        in.exceptions( std::ifstream::failbit );

        try
        {
            std::string magic( 6, '\0' );
            in.read( &magic[ 0 ], 6 );
            if ( magic != MAGIC_1_0 )
            {
                printf( "[ERROR] File is not a tbin file.\n" );
                return false;
            }

            std::string id = read< std::string >( in );
            std::string desc = read< std::string >( in );
            Properties props = readProperties( in );

            std::vector< TileSheet > tilesheets;
            int tilesheetCount = read< std::int32_t >( in );
            for ( int i = 0; i < tilesheetCount; ++i )
            {
                tilesheets.push_back( readTilesheet( in ) );
            }

            std::vector< Layer > layers;
            int layerCount = read< std::int32_t >( in );
            for ( int i = 0; i < layerCount; ++i )
            {
                layers.push_back( readLayer( in ) );
            }

            std::swap( this->id, id );
            std::swap( this->desc, desc );
            std::swap( this->props, props );
            std::swap( this->tilesheets, tilesheets );
            std::swap( this->layers, layers );

            return true;
        }
        catch ( std::exception& e )
        {
            printf( "[ERROR] Exception reading: %s\n", e.what() );
            return false;
        }
    }

    bool Map::saveToFile( const std::string& path ) const
    {
        std::ofstream file( path, std::ofstream::binary | std::ofstream::trunc );
        if ( !file )
        {
            printf( "[ERROR] %s: Failed to open file.\n", path.c_str() );
            return false;
        }

        printf( "[INFO] Writing to %s...\n", path.c_str() );
        return saveToStream( file );
    }

    bool Map::saveToStream( std::ostream& out ) const
    {
        out.exceptions( std::ifstream::failbit );
        try
        {
            out.write( MAGIC_1_0, 6 );

            write( out, id );
            write( out, desc );
            writeProperties( out, props );

            write< std::int32_t >( out, tilesheets.size() );
            for ( const TileSheet& ts : tilesheets )
                writeTilesheet( out, ts );

            write< std::int32_t >( out, layers.size() );
            for ( const Layer& layer : layers )
                writeLayer( out, layer );

            return true;
        }
        catch ( std::exception& e )
        {
            printf( "[ERROR] Exception writing: %s\n", e.what() );
            return false;
        }
    }
}
