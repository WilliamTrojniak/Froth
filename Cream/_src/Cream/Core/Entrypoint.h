#pragma once
#include "Cream/Core/Application.h"
#include "Cream/Utility/Utility.h"

extern Cream::Ref<Cream::Application> Cream::CreateApplication();

int main(int argc, char** argv)
{
	Cream::Ref<Cream::Application> app = Cream::CreateApplication();
	return 0;
}