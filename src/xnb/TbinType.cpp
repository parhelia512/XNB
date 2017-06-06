#include "xnb/TbinType.hpp"

#include "xnb/Util.hpp"

namespace xnb
{
    std::string TbinData::toString() const
    {
        return "Tbin";
    }
    
    std::string TbinData::getType() const
    {
        return std::string( TBIN_TYPE ) + ", xTile";
    }
    
    TypeReaderHeader TbinData::getTypeReader() const
    {
        return TypeReaderHeader{ std::string( TBIN_TYPEREADER ) + ", xTile", 0 };
    }
    
    std::unique_ptr< Data > TbinTypeReader::read( const File& file, std::istream& in, const std::string& fullDecl )
    {
        std::unique_ptr< Data > data( new TbinData() );
        TbinData* tbin = static_cast< TbinData* >( data.get() );
        
        sf::Int32 byteCount;
        in.read( reinterpret_cast< char* >( &byteCount ), 4 );
        tbin->data.resize( byteCount, '\0' );
        in.read( &tbin->data[ 0 ], byteCount );
        
        return std::move( data );
    }
    
    void TbinTypeReader::write( const File& file, std::ostream& out, const Data* data )
    {
        const TbinData* tbin = dynamic_cast< const TbinData* >( data );
        if ( tbin == nullptr )
            throw std::invalid_argument( "Bad data type passed to write" );
        
        sf::Int32 byteCount = tbin->data.size();
        out.write( reinterpret_cast< char* >( &byteCount ), 4 );
        out.write( tbin->data.c_str(), tbin->data.size() );
    }
    
    bool TbinTypeReader::resultIsValueType() const
    {
        return false;
    }
    
    std::string TbinTypeReader::getReaderType() const
    {
        return TBIN_TYPEREADER;
    }
}
