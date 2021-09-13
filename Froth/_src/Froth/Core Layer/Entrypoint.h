#pragma once
#include "Froth/Core Layer/Application.h"
#include "FMath/FMath.h"
#include "Froth/Platform Independence Layer/Window/WindowsWindow.h"



extern Froth::Application* Froth::CreateApplication();
int main(int argc, char** argv)
{

	
	
	
	//Froth::WindowsWindow window = Froth::WindowsWindow();
	//window.create();


	//std::cout << "Running the rest of the application";
	
	auto app = Froth::CreateApplication();
	app->Run();

	delete app;

	return 0;
} 