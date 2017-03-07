#include "build.h"
#include "absolutePath.h"
#include "fileUtils.h"

#include <string>

namespace sc
{
	namespace helper
	{
		Bool IsLetter( char c )
		{
			return ( c >= 'A' && c <= 'Z' ) || ( c >= 'a' && c <= 'z' );
		}

		void ValidateSlashes( const char* path )
		{
			const Uint32 len = static_cast< Uint32 >( strlen( path ) );
			for ( Uint32 i = 0; i < len; ++i )
			{
				SC_ASSERT( path[ i ] != '/', "\\ should be used instead of /" );
			}
		}

		std::string ToLower( const char* text )
		{
			std::string result( text );
			
			const Uint32 len = ( Uint32 )result.length();
			for ( Uint32 i = 0; i < len; ++i )
			{
				if ( result[ i ] <= 'Z' && result[ i ] >= 'A' )
				{
					result[ i ] += ( 'z' - 'Z' );
				}
			}

			return result;
		}
	}

	AbsolutePath::AbsolutePath( const char* path )
		: m_path( path )
	{
		Reparse();
	}

	AbsolutePath::~AbsolutePath()
	{
	}

	Bool AbsolutePath::IsDirectory() const
	{
		return path::IsDirectory( m_path.c_str() );
	}

	const char* AbsolutePath::GetFileName() const
	{
		return m_path.c_str() + m_fileNameIndex;
	}

	void AbsolutePath::AppendDirectory( const char* dirName )
	{
		if ( IsDirectory() )
		{
			std::string dirStr = helper::ToLower( dirName );
			const Uint32 len = ( Uint32 )dirStr.length();

			if ( dirStr[ len - 1 ] == '/' )
			{
				dirStr[ len - 1 ] = '\\';
			}
			else if ( dirStr[ len - 1 ] != '\\' )
			{
				dirStr.append( "\\" );
			}

			m_path.append( dirName );
		}
		else
		{
			SC_ASSERT( false, "Directory can be only added for a directory path." );
		}
	}

	void AbsolutePath::SetFile( const char* fileName )
	{
		if ( IsDirectory() )
		{
			m_path.append( fileName );
		}
		else
		{
			SC_ASSERT( false, "File name can be only set for a directory path." );
		}
	}

	void AbsolutePath::Reparse()
	{
		const char* cIter = m_path.c_str();
		
		SC_ASSERT( helper::IsLetter( cIter[ 0 ] ), "Absolute path has to start with a disk letter." );
		SC_ASSERT( cIter[ 1 ] == ':', "Absolute path has to start with a disk letter." );
		SC_ASSERT( cIter[ 2 ] == '\\', "Absolute path has to start with a disk letter." );

		// find filename index
		Uint32 fileIndex = static_cast< Uint32 >( strlen( cIter ) ) - 1;

		if ( IsDirectory() )
		{
			--fileIndex;
		}

		for ( ;; )
		{
			if ( cIter[ fileIndex ] == '\\' || cIter[ fileIndex ] == '/' )
			{
				++fileIndex;
				break;
			}

			--fileIndex;
		}

		m_fileNameIndex = fileIndex;

		SC_VALIDATE( helper::ValidateSlashes( cIter ) );
	}

}