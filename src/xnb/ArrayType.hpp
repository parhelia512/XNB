#ifndef XNB_ARRAYTYPE_HPP
#define XNB_ARRAYTYPE_HPP

#include <memory>
#include <string>
#include <vector>

#include "xnb/Data.hpp"
#include "xnb/ITypeReader.hpp"

namespace xnb
{
    class ArrayData : public Data
    {
        public:
            ArrayData( const std::string& theType );
            
            const std::string type;
            
            std::vector< std::unique_ptr< Data > > data;
            
            virtual std::string toString() const override;
            
            virtual std::vector< TypeReaderHeader > getTypeReaders() const override;
            virtual std::string getType() const override;
            virtual TypeReaderHeader getTypeReader() const override;
    };
    
    class ArrayTypeReader : public ITypeReader
    {
        public:
            virtual std::unique_ptr< Data > read( const File& file, std::istream& in, const std::string& fullDecl ) override;
            virtual void write( const File& file, std::ostream& out, const Data* data ) override;
            
            virtual bool resultIsValueType() const override;
            virtual std::string getReaderType() const override;
    };
}

#endif // XNB_ARRAYTYPE_HPP
