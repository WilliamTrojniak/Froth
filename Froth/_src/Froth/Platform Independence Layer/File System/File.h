#pragma once
#include "FilePath.h"

// Wrapper class for std file libraries

namespace Froth
{
	class File
	{
	public:
		File(const FilePath& path);
		File(const char* path);

		const FilePath& path() const;
		
		bool is_open();

		void open(I32 flags = std::ios::in | std::ios::out);
		void close();

		std::streampos size();

		std::string read();

		void write(const char* buffer, const size_t& sizeBytes);

	private:
		FilePath m_Path;
		std::fstream m_Stream;
	};
}