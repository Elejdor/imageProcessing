#pragma once
#include "stringRange.h"

#include <string>
#include <memory>

namespace sc
{
	class AbsolutePath
	{
	public:
		AbsolutePath( const char* path );
		~AbsolutePath();

		Bool IsDirectory() const;
		const char* GetFileName() const;
		void AppendDirectory( const char* dirName );
		void SetFile( const char* fileName );

	private:
		void Reparse();

	private:
		std::string		m_path;
		Uint32			m_fileNameIndex;
	};
}