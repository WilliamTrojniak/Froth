#include <frothpch.h>
#include "File.h"

namespace Froth
{
	File::File(const FilePath& path)
		: m_Path(std::forward<const FilePath&>(path)), m_Stream()
	{
	}

	File::File(const char* path)
		: m_Path(path)
	{
	}
	const FilePath& File::path() const
	{
		return m_Path;
	}

	bool File::is_open()
	{
		return m_Stream.is_open();
	}

	void File::open(I32 flags)
	{
		m_Stream.open(m_Path.string(), flags);
	}

	void File::close()
	{
		m_Stream.close();
	}

	std::streampos File::size()
	{
		m_Stream.seekg(0, std::ios::end);
		std::streampos size = m_Stream.tellg();

		m_Stream.seekg(std::ios::beg);
		
		return size;
	}
	
	std::string File::read()
	{
		if (m_Stream.is_open())
		{
			std::string outStr;
			outStr.resize(this->size());
		
			m_Stream.read(outStr.data(), this->size());

			return outStr;
		}
		return "";
	}
	void File::write(const char* buffer, const size_t& sizeBytes)
	{
		m_Stream.write(buffer, sizeBytes);

	}
}

