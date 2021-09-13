#include "frothpch.h"
#include "WindowAPI.h"

namespace Froth
{
	
#ifdef FROTH_WINDOWAPI_WIN
	WindowAPI::API WindowAPI::s_API = WindowAPI::API::WIN;
#else
	WindowAPI::API WindowAPI::s_API = WindowAPI::API::NONE;
#endif

}