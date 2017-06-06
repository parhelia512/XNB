#include "xnb/TextureType.hpp"

#include <util/String.hpp>

#include "xnb/Util.hpp"

namespace xnb
{
    enum TextureFormat : sf::Int32
    {
        Color = 0,
        Bgr565 = 1,
        Bgra5551 = 2,
        Bgra4444 = 3,
        Dxt1 = 4,
        Dxt3 = 5,
        Dxt5 = 6,
        NormalizedByte2 = 7,
        NormalizedByte4 = 8,
        Rgba1010102 = 9,
        Rg32 = 10,
        Rgba64 = 11,
        Alpha8 = 12,
        Single = 13,
        Vector2 = 14,
        Vector4 = 15,
        HalfSingle = 16,
        HalfVector2 = 17,
        HalfVector4 = 18,
        HdrBlendable = 19,
    };
    
    std::string Texture2DData::toString() const
    {
        return util::format( "Texture2D{($, $) x $}", data[ 0 ].getSize().x, data[ 0 ].getSize().y, data.size() );
    }
    
    std::string Texture2DData::getType() const
    {
        return std::string( TEXTURE2D_TYPE ) + ", Microsoft.Xna.Framework.Graphics";
    }
    
    TypeReaderHeader Texture2DData::getTypeReader() const
    {
        return TypeReaderHeader{ std::string( TEXTURE2D_TYPEREADER ) + ", Microsoft.Xna.Framework.Graphics", 0 };
    }
    
    std::unique_ptr< Data > Texture2DTypeReader::read( const File& file, std::istream& in, const std::string& fullDecl )
    {
        std::unique_ptr< Data > data( new Texture2DData() );
        Texture2DData* texs = static_cast< Texture2DData* >( data.get() );
        
        sf::Int32 format;
        in.read( reinterpret_cast< char* >( &format ), 4 );
        
        sf::Uint32 width, height, mips;
        in.read( reinterpret_cast< char* >( &width ), 4 );
        in.read( reinterpret_cast< char* >( &height ), 4 );
        in.read( reinterpret_cast< char* >( &mips ), 4 );
        
        // So I have no idea what to do here. I assume these are along the lines of mip maps, so I doubt they're all the same size?
        // Meaning I can't just use the same code below to create images
        if ( mips > 1 )
            throw std::invalid_argument( util::format( "More than one mips not implemented." ) );
        
        texs->data.reserve( mips );
        for ( int i = 0; i < mips; ++i )
        {
            sf::Uint32 byteCount;
            in.read( reinterpret_cast< char* >( &byteCount ), 4 );
            std::vector< sf::Uint8 > bytes( byteCount );
            in.read( reinterpret_cast< char* >( &bytes[ 0 ] ), byteCount );
            
            texs->data.push_back( sf::Image() );
            switch ( format )
            {
                case Color:
                    texs->data[ i ].create( width, height, &bytes[ 0 ] );
                    break;
                default:
                    throw std::invalid_argument( util::format( "Texture format not supported: $\n", format ) );
            }
        }
        
        return std::move( data );
    }
    
    void Texture2DTypeReader::write( const File& file, std::ostream& out, const Data* data )
    {
        const Texture2DData* tex = dynamic_cast< const Texture2DData* >( data );
        if ( tex == nullptr )
            throw std::invalid_argument( "Bad data type passed to write" );
        
        sf::Int32 format = Color;
        sf::Uint32 width = tex->data[ 0 ].getSize().x;
        sf::Uint32 height = tex->data[ 0 ].getSize().y;
        sf::Uint32 mips = tex->data.size();
        out.write( reinterpret_cast< const char* >( &format ), 4 );
        out.write( reinterpret_cast< const char* >( &width ), 4 );
        out.write( reinterpret_cast< const char* >( &height ), 4 );
        out.write( reinterpret_cast< const char* >( &mips ), 4 );
        
        for ( auto it = tex->data.begin(); it != tex->data.end(); ++it )
        {
            sf::Uint32 byteCount = it->getSize().x * it->getSize().y * 4;
            out.write( reinterpret_cast< const char* >( &byteCount ), 4 );
            out.write( reinterpret_cast< const char* >( it->getPixelsPtr() ), byteCount );
        }
    }
    
    bool Texture2DTypeReader::resultIsValueType() const
    {
        return false;
    }
    
    std::string Texture2DTypeReader::getReaderType() const
    {
        return TEXTURE2D_TYPEREADER;
    }
}
