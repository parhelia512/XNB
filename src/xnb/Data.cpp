#include "xnb/Data.hpp"

namespace xnb
{
    /*
    Data::Data( const std::string& theType )
    :   type( theType )
    {
    }
    */
    
    Data::~Data()
    {
    }
    
    std::vector< TypeReaderHeader > Data::getTypeReaders() const
    {
        return { getTypeReader() };
    }
}
