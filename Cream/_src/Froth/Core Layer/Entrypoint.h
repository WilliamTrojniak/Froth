#pragma once
#include "Froth/Core Layer/Application.h"

// Temp
#include <iostream>
#include <string>
#include <vector>

struct Vector3
{
	float x = 0.0f, y = 0.0f, z = 0.0f;
	int* m_MemoryBlock = nullptr;


	Vector3()
	{
		m_MemoryBlock = new int[5];
		std::cout << "Default Constructor\n";
	}
	Vector3(float scalar)
		: x(scalar), y(scalar), z(scalar)
	{
		std::cout << "Scalar Constructor\n";
		m_MemoryBlock = new int[5];
	}
	Vector3(float x, float y, float z)
		: x(x), y(y), z(z)
	{
		std::cout << "Unique Constructor\n";
		m_MemoryBlock = new int[5];
	}

	Vector3(const Vector3& other)
	{
		std::cout << "Copy Constructor\n";
		x = other.x;
		y = other.y;
		z = other.z;
		m_MemoryBlock = new int[5];
		m_MemoryBlock[0] = other.m_MemoryBlock[0];
		m_MemoryBlock[1] = other.m_MemoryBlock[1];
		m_MemoryBlock[2] = other.m_MemoryBlock[2];
		m_MemoryBlock[3] = other.m_MemoryBlock[3];
		m_MemoryBlock[4] = other.m_MemoryBlock[4];
	}

	Vector3(Vector3&& other) noexcept
		: x(other.x), y(other.y), z(other.z)
	{
		m_MemoryBlock = other.m_MemoryBlock;
		other.m_MemoryBlock = nullptr;
		std::cout << "Move Constructor\n";
	}

	~Vector3()
	{
		delete m_MemoryBlock;
		std::cout << "Deconstructor\n";
	}

	Vector3& operator=(const Vector3& other)
	{
		std::cout << "Copy Operator\n";
		x = other.x;
		y = other.y;
		z = other.z;
		m_MemoryBlock = new int[5];
		m_MemoryBlock[0] = other.m_MemoryBlock[0];
		m_MemoryBlock[1] = other.m_MemoryBlock[1];
		m_MemoryBlock[2] = other.m_MemoryBlock[2];
		m_MemoryBlock[3] = other.m_MemoryBlock[3];
		m_MemoryBlock[4] = other.m_MemoryBlock[4];
	}


	Vector3& operator=(Vector3&& other) noexcept
	{
		std::cout << "Move Operator\n";
		x = other.x;
		y = other.y;
		z = other.z;
		m_MemoryBlock = other.m_MemoryBlock;
		other.m_MemoryBlock = nullptr;
		return *this;
	}
};

extern Froth::Ref<Froth::Application> Froth::CreateApplication();
int main(int argc, char** argv)
{
	
	Froth::CreateApplication();

	
	return 0;
} 