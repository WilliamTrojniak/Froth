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

		void open();
		void close();

		std::streampos size();

		std::string read();

	private:
		FilePath m_Path;
		std::fstream m_Stream;
	};
}