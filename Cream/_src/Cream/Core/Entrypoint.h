#pragma once
#include <iostream>
#include <Logger.h>

extern void HelloWorld();

int main(int argc, char** argv)
{
	std::string s = "Hello";
	Logger::Print(s);
	HelloWorld();
	return 0;
}