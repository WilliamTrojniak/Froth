#pragma once
#include "Cream/Utility/Utility.h"

namespace Cream
{
	class Application
	{
	public:
		Application();
		~Application();

	};

	Ref<Application> CreateApplication();
}