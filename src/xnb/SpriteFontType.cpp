#include "xnb/SpriteFontType.hpp"

//#include "xnb/CharacterType.hpp"
#include "xnb/ListType.hpp"
//#include "xnb/RectangleType.hpp"
#include "xnb/TextureType.hpp"
#include "xnb/Util.hpp"

namespace xnb
{
    std::string SpriteFontData::toString() const
    {
        return "SpriteFont";
    }
    
    std::unique_ptr< Data > SpriteFontTypeReader::read( const File& file, std::istream& in, const std::string& fullDecl )
    {
        std::unique_ptr< Data > data( new SpriteFontData() );
        SpriteFontData* font = static_cast< SpriteFontData* >( data.get() );
        
        ITypeReader* image = ITypeReader::getTypeReader( TEXTURE2D_TYPE );
        ITypeReader* list = ITypeReader::getTypeReader( LIST_TYPE );
        //ITypeReader* nullable = ITypeReader::getTypeReader( NULLABLE_TYPE );
        
        auto tex = image->read( file, in, TEXTURE2D_TYPEREADER );
        
        font->tex = static_cast< Texture2DData* >( tex.get() )->data[ 0 ];
        // ...
        
        return std::move( data );
    }
    
    bool SpriteFontTypeReader::resultIsValueType() const
    {
        return false;
    }
}
