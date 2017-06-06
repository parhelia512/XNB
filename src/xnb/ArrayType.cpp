#include "xnb/ArrayType.hpp"

#include <util/String.hpp>

#include "xnb/Util.hpp"

namespace xnb
{
    ArrayData::ArrayData( const std::string& theType )
    :   type( theType )
    {
    }
    
    std::string ArrayData::toString() const
    {
        std::string str = "Array{";
        for ( auto it = data.begin(); it != data.end(); ++it )
        {
            str += it->get()->toString() + ',';
        }
        str[ str.length() - 1 ] = '}';
        return str;
    }
    
    std::vector< TypeReaderHeader > ArrayData::getTypeReaders() const
    {
        std::vector< TypeReaderHeader > ret;
        ret.push_back( getTypeReader() );
        ret.push_back( TypeReaderHeader{ ITypeReader::getTypeReader( type )->getReaderType() } );
        
        for ( std::size_t id = 0; id < data.size(); ++id )
        {
            std::vector< TypeReaderHeader > trhs = data[ id ]->getTypeReaders();
            for ( std::size_t itr = 0; itr < trhs.size(); ++itr )
            {
                bool dup = false;
                for ( std::size_t ir = 0; ir < ret.size(); ++ir )
                {
                    if ( ret[ ir ] == trhs[ itr ] )
                    {
                        dup = true;
                        break;
                    }
                }
                
                if ( !dup )
                {
                    ret.push_back( trhs[ itr ] );
                }
            }
        }
        
        return std::move( ret );
    }
    
    std::string ArrayData::getType() const
    {
        return "[]"; // What else to do?
    }
    
    TypeReaderHeader ArrayData::getTypeReader() const
    {
        return TypeReaderHeader{ "Microsoft.Xna.Framework.Content.ArrayReader", 0 };
    }
    
    std::unique_ptr< Data > ArrayTypeReader::read( const File& file, std::istream& in, const std::string& fullDecl )
    {
        std::string type = fullDecl.substr( 0, fullDecl.length() - 2 );
        std::unique_ptr< Data > data( new ArrayData( type ) );
        ArrayData* arr = static_cast< ArrayData* >( data.get() );
        
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
    
    void ArrayTypeReader::write( const File& file, std::ostream& out, const Data* data )
    {
        const ArrayData* arr = dynamic_cast< const ArrayData* >( data );
        if ( arr == nullptr )
            throw std::invalid_argument( "Bad data type passed to write" );
        
        write7BitEncodedInt( out, arr->data.size() );
        for ( const auto& dataVal : arr->data )
        {
            ITypeReader* reader = ITypeReader::getTypeReader( dataVal->getType() );
            if ( reader == nullptr )
                throw std::invalid_argument( util::format( "Not supported: $", dataVal->getType() ) );
                
            if ( !reader->resultIsValueType() )
            {
                write7BitEncodedInt( out, getIndexOfTypeReader( file, dataVal->getTypeReader().name ) + 1 );
                reader->write( file, out, dataVal.get() );
            }
            else reader->write( file, out, dataVal.get() );
        }
    }
    
    bool ArrayTypeReader::resultIsValueType() const
    {
        return false;
    }
    
    std::string ArrayTypeReader::getReaderType() const
    {
        return "Microsoft.Xna.Framework.Content.ArrayReader";
    }
}
