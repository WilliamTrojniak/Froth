#pragma once
#include <frothpch.h>

namespace Froth
{
	class Timer
	{
	public:
		Timer(const char* name)
			: m_Name(name), m_Stopped(false)
		{
			m_StartTimepoint = std::chrono::high_resolution_clock::now();
		}

		~Timer()
		{
			if (!m_Stopped)
				stop();
		}

		void stop()
		{
			std::chrono::time_point<std::chrono::steady_clock> endTimepoint = std::chrono::high_resolution_clock::now();

			U64 start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
			U64 end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

			m_Stopped = true;
			F32 duration = (end - start) * 0.001f;
			std::cout << m_Name << ": " << duration << "ms" << std::endl;
		}

	private:
		const char* m_Name;
		std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
		bool m_Stopped;
	};
}
