#ifndef XNB_TBINTYPE_HPP
#define XNB_TBINTYPE_HPP

#include <memory>
#include <string>

#include "xnb/Data.hpp"
#include "xnb/ITypeReader.hpp"

namespace xnb
{
    class TbinData : public Data
    {
        public:
            std::string data;
            
            virtual std::string toString() const override;
            
            virtual std::string getType() const override;
            virtual TypeReaderHeader getTypeReader() const override;
    };
    
    class TbinTypeReader : public ITypeReader
    {
        public:
            virtual std::unique_ptr< Data > read( const File& file, std::istream& in, const std::string& fullDecl ) override;
            virtual void write( const File& file, std::ostream& out, const Data* data ) override;
            
            virtual bool resultIsValueType() const override;
            virtual std::string getReaderType() const override;
    };
    
    constexpr const char* TBIN_TYPE = "xTile.Map";
    constexpr const char* TBIN_TYPEREADER = "xTile.Pipeline.TideReader";
}

#endif // XNB_TBINTYPE_HPP
