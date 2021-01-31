#pragma once
#include <frothpch.h>

namespace Froth
{
	class FilePath
	{
	public:
		FilePath(const char* path);

		FilePath root_name() const;

		FilePath root_directory() const;

		FilePath root_path() const;

		FilePath filename() const;

		FilePath extension() const;

		std::string string() const;

		friend std::ostream& operator<<(std::ostream& os, const Froth::FilePath& filePath);

	private:
		std::filesystem::path m_Path;

	
	};
}
