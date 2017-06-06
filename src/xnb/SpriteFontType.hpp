#ifndef XNB_SPRITEFONTTYPE_HPP
#define XNB_SPRITEFONTTYPE_HPP

#include <memory>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector3.hpp>
#include <string>
#include <vector>

#include "xnb/Data.hpp"
#include "xnb/ITypeReader.hpp"

namespace xnb
{
    class SpriteFontData : public Data
    {
        public:
            sf::Image tex;
            std::vector< sf::FloatRect > glyphs;
            std::vector< sf::FloatRect > croppings;
            std::vector< char > charMap;
            sf::Int32 lineSpacing;
            float spacing;
            std::vector< sf::Vector3f > kerning;
            std::unique_ptr< char > defaultChar;
            
            virtual std::string toString() const override;
            
            //virtual std::vector< TypeReaderHeader > getTypeReaders() const override;
            //virtual std::string getType() const override;
            //virtual TypeReaderHeader getTypeReader() const override;
    };
    
    class SpriteFontTypeReader : public ITypeReader
    {
        public:
            virtual std::unique_ptr< Data > read( const File& file, std::istream& in, const std::string& fullDecl ) override;
            //virtual void write( const File& file, std::ostream& out, const Data* data ) override;
            
            virtual bool resultIsValueType() const override;
            //virtual std::string getReaderType() const override;
    };
    
    constexpr const char* SPRITEFONT_TYPE = "Microsoft.Xna.Framework.Graphics.SpriteFont";
    constexpr const char* SPRITEFONT_TYPEREADER = "Microsoft.Xna.Framework.Content.SpriteFontReader";
}

#endif // XNB_SPRITEFONTTYPE_HPP
