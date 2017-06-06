#ifndef XNB_LISTTYPE_HPP
#define XNB_LISTTYPE_HPP

#include <memory>
#include <string>
#include <vector>

#include "xnb/Data.hpp"
#include "xnb/ITypeReader.hpp"

namespace xnb
{
    class ListData : public Data
    {
        public:
            ListData( const std::string& theType );
            
            const std::string type;
            
            std::vector< std::unique_ptr< Data > > data;
            
            virtual std::string toString() const override;
            
            virtual std::vector< TypeReaderHeader > getTypeReaders() const override;
            virtual std::string getType() const override;
            virtual TypeReaderHeader getTypeReader() const override;
    };
    
    class ListTypeReader : public ITypeReader
    {
        public:
            virtual std::unique_ptr< Data > read( const File& file, std::istream& in, const std::string& fullDecl ) override;
            virtual void write( const File& file, std::ostream& out, const Data* data ) override;
            
            virtual bool resultIsValueType() const override;
            virtual std::string getReaderType() const override;
    };
    
    constexpr const char* LIST_TYPE = "System.Collections.Generic.List";
    constexpr const char* LIST_TYPEREADER = "Microsoft.Xna.Framework.Content.ListReader";
}

#endif // XNB_LISTTYPE_HPP
