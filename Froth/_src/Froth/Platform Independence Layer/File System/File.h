#pragma once
#include "FilePath.h"

// Wrapper class for std file libraries

namespace Froth
{
	class File
	{
	public:
		File();
		File(const FilePath& path);
		File(const char* path);

		const FilePath& path() const;
		void setPath(const FilePath& path);
		void setPath(const char* path);
		
		bool is_open();

		void open(I32 flags = std::ios::in | std::ios::out);
		void flush();
		void close();

		std::streampos size();

		std::string read();

		void write(const char* buffer, const size_t& sizeBytes);

		std::fstream& operator << (const char* vals)
		{
			m_Stream << vals;
			return m_Stream;
		}

	private:
		FilePath m_Path;
		std::fstream m_Stream;
	};
}