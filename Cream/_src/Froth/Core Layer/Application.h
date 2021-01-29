#pragma once
#include "Base.h"

namespace Froth
{
	class Application
	{
	public:
		Application();
		~Application();

	};

	Ref<Application> CreateApplication();
}