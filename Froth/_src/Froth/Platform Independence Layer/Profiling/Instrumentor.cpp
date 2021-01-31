#include "frothpch.h"
#include "Instrumentor.h"


namespace Froth
{
	Instrumentor::Instrumentor()
		: m_CurrentSession(nullptr), m_OutputFile(""), m_ProfileCount(0)
	{

	}

	void Instrumentor::beginSession(const std::string& name, const char* filepath)
	{
		m_OutputFile.setPath(filepath);
		m_OutputFile.open(std::iostream::out | std::iostream::trunc);
		m_OutputFile.close();
		m_OutputFile.open(std::iostream::out | std::iostream::app);
		writeHeader();
		m_CurrentSession = new InstrumentationSession{ name };
	}

	void Instrumentor::endSession()
	{
		writeFooter();
		m_OutputFile.close();
		delete m_CurrentSession;
		m_CurrentSession = nullptr;
		m_ProfileCount = 0;
	}

	void Instrumentor::writeProfile(const ProfileResult& result)
	{
		if (m_ProfileCount++ > 0)
			m_OutputFile << ",";
	
		//std::replace(name.begin(), name.end(), '"', '\'');

		m_OutputFile << "{";
		m_OutputFile << "\"name\":";
		m_OutputFile << "\"" << result.Name.data() << "\",";
		m_OutputFile << "\"start\":\"" << result.Start << "\",";
		m_OutputFile << "\"end\":\"" << result.End << "\"";
		m_OutputFile << "}";

		m_OutputFile.flush();
	}

	void Instrumentor::writeHeader()
	{
		m_OutputFile << "{\"traceEvents\":[";
		m_OutputFile.flush();
	}

	void Instrumentor::writeFooter()
	{
		m_OutputFile << "]}";
		m_OutputFile.flush();
	}

	Instrumentor& Instrumentor::get()
	{
		static Instrumentor instance;
		return instance;
	}
}