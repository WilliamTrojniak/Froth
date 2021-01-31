#pragma once
#include "Froth/Core Layer/Application.h"

extern Froth::Ref<Froth::Application> Froth::CreateApplication();
int main(int argc, char** argv)
{
	Froth::File file = Froth::File("D:\\sample data\\data - Copy.json");
	
	file.open();

	//char* buffer = new char[(U64)file.size() + 1];
	//buffer[file.size()] = '\0';

	std::string out = file.read();

	file.close();

	std::cout << out.data();


	//delete[] buffer;



	//std::cout << file.size();


	//Froth::CreateApplication();

	return 0;
} 