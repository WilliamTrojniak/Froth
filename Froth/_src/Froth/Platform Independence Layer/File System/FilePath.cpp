#include <frothpch.h>
#include "FilePath.h"

namespace Froth
{
    FilePath::FilePath(const char* path)
       : m_Path(path)
    {
    }

	FilePath FilePath::root_name() const
	{
		return FilePath(m_Path.root_name().string().c_str());
	}

	FilePath FilePath::root_directory() const
	{
		return FilePath(m_Path.root_directory().string().c_str());
	}

	FilePath FilePath::root_path() const
	{
		return FilePath(m_Path.root_path().string().c_str());
	}

	FilePath FilePath::filename() const
	{
		return FilePath(m_Path.filename().string().c_str());
	}

	FilePath FilePath::extension() const
	{
		return FilePath(m_Path.extension().string().c_str());
	}

	std::string FilePath::string() const
	{
		return m_Path.string();
	}


	std::ostream& operator<<(std::ostream& os, const Froth::FilePath& filePath)
	{
		os << filePath.m_Path;
		return os;
	}

}