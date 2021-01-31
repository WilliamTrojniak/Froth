#pragma once
#include <frothpch.h>
#include "Froth/Platform Independence Layer/File System/File.h"


namespace Froth
{
	class Instrumentor
	{
	private:
		struct ProfileResult
		{
			std::string Name;
			U64 Start, End;
		};

		struct InstrumentationSession
		{
			std::string Name;
		};

	public:

		Instrumentor();

		void beginSession(const std::string& name, const char* filepath = "results.json");
		void endSession();

		void writeProfile(const ProfileResult& result);

		static Instrumentor& get();

	private:
		void writeHeader();
		void writeFooter();

		

	private:
		InstrumentationSession* m_CurrentSession;
		File m_OutputFile;
		U32 m_ProfileCount;
	};
}