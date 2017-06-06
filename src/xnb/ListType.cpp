#include "xnb/ListType.hpp"

#include "xnb/Util.hpp"

namespace xnb
{
    ListData::ListData( const std::string& theType )
    :   type( theType )
    {
    }
    
    std::string ListData::toString() const
    {
        std::string str = "Array{";
        for ( auto it = data.begin(); it != data.end(); ++it )
        {
            str += it->get()->toString() + ',';
        }
        str[ str.length() - 1 ] = '}';
        return str;
    }
    
    std::unique_ptr< Data > ListTypeReader::read( const File& file, std::istream& in, const std::string& fullDecl )
    {
        std::size_t startGeneric = fullDecl.find( '`' );
        std::size_t startType = startGeneric + 4;
        std::size_t endType = fullDecl.find( ',', startGeneric + 4 );
        
        // tODO: These won't work with generics as is
        std::string type = fullDecl.substr( startType, endType - startType );
        
        std::unique_ptr< Data > data( new ListData( type ) );
        ListData* arr = static_cast< ListData* >( data.get() );
        
        sf::Uint32 count;
        in.read( reinterpret_cast< char* >( &count ), sizeof( count ) );
        for ( std::size_t i = 0; i < count; ++i )
        {
            std::unique_ptr< Data > value;
            auto reader = ITypeReader::getTypeReader( type );
            if ( !reader->resultIsValueType() )
            {
                int properReader = read7BitEncodedInt( in );
                if ( properReader != 0 )
                {
                    reader = ITypeReader::getTypeReader( getTypeReaderNameAtIndex( file, properReader - 1 ) );
                    value = reader->read( file, in, type );
                }
            }
            else value = reader->read( file, in, type );
            
            arr->data.emplace_back( std::move( value ) );
        }
        
        return std::move( data );
    }
    
    bool ListTypeReader::resultIsValueType() const
    {
        return false;
    }
}
