#include "Cream/Core/Entrypoint.h"
#include <iostream>

class Playground : public Cream::Application
{
public:
	Playground()
	{
		std::cout << "Playground created!" << std::endl;
	}
};

Cream::Ref<Cream::Application> Cream::CreateApplication()
{
	return Cream::CreateRef<Cream::Application>(Playground());
}