#pragma once
namespace Froth
{
	class WindowAPI
	{
	public:

		enum class API
		{
			NONE = 0,
			WIN = 1
		};

	private:
		static API s_API;

	public:
		inline static API getAPI() { return s_API; }


	};
}