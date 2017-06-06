#ifndef XNB_TEXTURETYPE_HPP
#define XNB_TEXTURETYPE_HPP

#include <memory>
#include <SFML/Graphics/Image.hpp>
#include <string>

#include "xnb/Data.hpp"
#include "xnb/ITypeReader.hpp"

namespace xnb
{
    class Texture2DData : public Data
    {
        public:
            std::vector< sf::Image > data;
            
            virtual std::string toString() const override;
            
            virtual std::string getType() const override;
            virtual TypeReaderHeader getTypeReader() const override;
    };
    
    class Texture2DTypeReader : public ITypeReader
    {
        public:
            virtual std::unique_ptr< Data > read( const File& file, std::istream& in, const std::string& fullDecl ) override;
            virtual void write( const File& file, std::ostream& out, const Data* data ) override;
            
            virtual bool resultIsValueType() const override;
            virtual std::string getReaderType() const override;
    };
    
    constexpr const char* TEXTURE2D_TYPE = "Microsoft.Xna.Framework.Graphics.Texture2D";
    constexpr const char* TEXTURE2D_TYPEREADER = "Microsoft.Xna.Framework.Content.Texture2DReader";
}

#endif // XNB_TEXTURETYPE_HPP
