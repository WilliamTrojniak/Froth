#include "creampch.h"
#include "String.h"

namespace Cream
{
	size_t String::len(const char* const str)
	{
		if (!str)
			return 0;

		size_t index = 0;
		while (str[index] != '\0')
			index++;
		return index;
	}

	void String::cpy(char* const dest, const char* const src)
	{
		if (!src || !dest)
			return;

		size_t index = 0;
		while (src[index] != '\0')
		{
			dest[index] = src[index];
			index++;
		}
		dest[index] = '\0';
	}

	void String::cpy(char* const dest, const String& src)
	{
		memcpy(dest, src.m_StrData, src.m_Length + 1);
	}

	void String::cat(char* const dest, const char* const src1, const char* const src2)
	{
		if (!dest)
			return;
		size_t globalIndex = 0;
		if (src1)
		{
			size_t i = 0;
			while (src1[i] != '\0')
			{
				dest[globalIndex] = src1[i];
				i++;
				globalIndex++;
			}
		}
		if (src2)
		{
			size_t i = 0;
			while (src2[i] != '\0')
			{
				dest[globalIndex] = src2[i];
				i++;
				globalIndex++;
			}
		}
		dest[globalIndex] = '\0';
	}

	void String::cat(char* const dest, const char* const src1, const String& src2)
	{
		if (!dest)
			return;
		size_t globalIndex = 0;
		if (src1)
		{
			size_t i = 0;
			while (src1[i] != '\0')
			{
				dest[globalIndex] = src1[i];
				i++;
				globalIndex++;
			}
		}
		memcpy(&dest[globalIndex], src2.m_StrData, src2.m_Length + 1);
	}

	void String::cat(char* const dest, const String& src1, const char* const src2)
	{
		if (!dest)
			return;

		memcpy(dest, src1.m_StrData, src1.m_Length);

		size_t globalIndex = src1.m_Length;
		if (src2)
		{
			size_t i = 0;
			while (src2[i] != '\0')
			{
				dest[globalIndex] = src2[i];
				i++;
				globalIndex++;
			}
		}
		dest[globalIndex] = '\0';
		
	}

	void String::cat(char* const dest, const String& src1, const String& src2)
	{
		if (!dest)
			return;
		memcpy(dest, src1.m_StrData, src1.m_Length);
		memcpy(&dest[src1.m_Length], src2.m_StrData, src2.m_Length+1);
	}

	String::String()
		: m_StrData(nullptr), m_Length(0)
	{
		m_StrData = new char[1];
		CREAM_ASSERT(m_StrData); // Check to ensure allocation completes
		m_StrData[0] = '\0';
	}

	String::String(const char& string)
		: m_Length(1)
	{
		m_StrData = new char[2];
		CREAM_ASSERT(m_StrData); // Check to ensure allocation completes
		m_StrData[0] = string;
		m_StrData[1] = '\0';
	}

	String::String(const char* const string)
	{
		if (string == nullptr)
		{
			m_Length = 0;
			m_StrData = new char[1];
			CREAM_ASSERT(m_StrData); // Check to ensure allocation completes
			m_StrData[0] = '\0';
		}
		else
		{
			m_Length = len(string);
			m_StrData = new char[m_Length + 1];
			CREAM_ASSERT(m_StrData); // Check to ensure allocation completes
			cpy(m_StrData, string);
		}
	}

	String::String(const String& otherStr)
		: m_Length(otherStr.m_Length)
	{
		m_Length = otherStr.m_Length;
		m_StrData = new char[m_Length + 1];
		CREAM_ASSERT(m_StrData); // Check to ensure allocation completes
		cpy(m_StrData, otherStr);
	}

	String::String(String&& otherStr) noexcept
		: m_Length(otherStr.m_Length)
	{
		m_StrData = otherStr.m_StrData;
		otherStr.m_StrData = nullptr;
	}

	String& String::operator=(const char& string)
	{
		delete m_StrData;
		m_Length = 1;
		m_StrData = new char[2];
		CREAM_ASSERT(m_StrData); // Check to ensure allocation completes
		m_StrData[0] = string;
		m_StrData[1] = '\0';
		return *this;
	}
	String& String::operator=(const char* const string)
	{
		delete m_StrData;
		if (string == nullptr)
		{
			m_Length = 0;
			m_StrData = new char[1];
			CREAM_ASSERT(m_StrData); // Check to ensure allocation completes
			m_StrData[0] = '\0';
		}
		else
		{
			m_Length = len(string);
			m_StrData = new char[m_Length + 1];
			CREAM_ASSERT(m_StrData); // Check to ensure allocation completes
			cpy(m_StrData, string);
		}
		return *this;
	}
	String& String::operator=(const String& otherStr)
	{
		if (this == &otherStr)
			return *this;

		delete m_StrData;

		m_Length = otherStr.m_Length;
		m_StrData = new char[m_Length + 1];
		CREAM_ASSERT(m_StrData); // Check to ensure allocation completes
		cpy(m_StrData, otherStr);

		return *this;
	}
	String& String::operator=(String&& otherStr) noexcept
	{
		delete m_StrData;

		m_Length = otherStr.m_Length;
		m_StrData = otherStr.m_StrData;
		otherStr.m_StrData = nullptr;

		return *this;
	}
	String& String::operator+=(const char& string)
	{
		pushBack(string);
		return *this;
	}
	String& String::operator+=(const char* const string)
	{
		pushBack(string);
		return *this;
	}
	String& String::operator+=(const String& otherStr)
	{
		pushBack(otherStr);
		return *this;
	}
	String String::operator+(const char& string)
	{
		String outStr = String();
		delete outStr.m_StrData;
		outStr.m_Length = m_Length + 1;
		outStr.m_StrData = new char[outStr.m_Length + 1];
		memcpy(outStr.m_StrData, m_StrData, m_Length);
		outStr.m_StrData[outStr.m_Length - 1] = string;
		outStr.m_StrData[outStr.m_Length] = '\0';
		return outStr;
	}
	String String::operator+(const char* const string)
	{
		String outStr = String();
		delete outStr.m_StrData;
		outStr.m_Length = m_Length + len(string);
		outStr.m_StrData = new char[outStr.m_Length + 1];
		cat(outStr.m_StrData, *this, string);
		return outStr;
	}
	String String::operator+(const String& otherStr)
	{
		String outStr = String();
		delete outStr.m_StrData;
		outStr.m_Length = m_Length + otherStr.m_Length;
		outStr.m_StrData = new char[outStr.m_Length + 1];
		cat(outStr.m_StrData, *this, otherStr);
		return outStr;
	}
	String operator+(const char& string, const String& otherStr)
	{
		String outStr = String();
		delete outStr.m_StrData;
		outStr.m_Length = otherStr.m_Length + 1;
		outStr.m_StrData = new char[outStr.m_Length + 1];
		memcpy(&outStr.m_StrData[1], otherStr.m_StrData, otherStr.m_Length);
		outStr.m_StrData[0] = string;
		outStr.m_StrData[outStr.m_Length] = '\0';
		return outStr;
	}
	String operator+(const char* const string, const String& otherStr)
	{
		String outStr = String();
		delete outStr.m_StrData;
		outStr.m_Length = otherStr.m_Length + String::len(string);
		outStr.m_StrData = new char[outStr.m_Length + 1];
		String::cat(outStr.m_StrData, string, otherStr);
		return outStr;
	}

	char& String::operator[](const size_t& index)
	{
		return m_StrData[index];
	}

	bool String::operator==(const char* const string)
	{
		return equals(string);
	}

	bool String::operator==(const String& string)
	{
		return equals(string.m_StrData);
	}

	bool String::operator!=(const char* const string)
	{
		return !equals(string);;
	}

	bool String::operator!=(const String& string)
	{
		return !equals(string.m_StrData);;
	}

	void String::pushBack(const char& string)
	{
		size_t newLength = m_Length + 1;
		char* newData = new char[newLength + 1];
		memcpy(newData, m_StrData, m_Length);
		newData[newLength - 1] = string;
		newData[newLength] = '\0';
		delete m_StrData;
		m_StrData = newData;
		m_Length = newLength;
	}
	void String::pushBack(const char* const string)
	{
		size_t newLength = m_Length + len(string);
		char* newData = new char[newLength + 1];
		cat(newData, *this, string);
		delete m_StrData;
		m_Length = newLength;
		m_StrData = newData;
	}
	void String::pushBack(const String& otherStr)
	{
		size_t newLength = m_Length + otherStr.m_Length;
		char* newData = new char[newLength + 1];
		cat(newData, *this, otherStr);
		delete m_StrData;
		m_Length = newLength;
		m_StrData = newData;
	}
	void String::pushFront(const char* const string)
	{
		size_t newLength = m_Length + len(string);
		char* newData = new char[newLength + 1];
		cat(newData, string, *this);
		delete m_StrData;
		m_Length = newLength;
		m_StrData = newData;
	}
	void String::pushFront(const String& otherStr)
	{
		size_t newLength = m_Length + otherStr.m_Length;
		char* newData = new char[newLength + 1];
		cat(newData, otherStr, *this);
		delete m_StrData;
		m_Length = newLength;
		m_StrData = newData;
	}

	bool String::equals(const char* const string) const
	{
		size_t index = 0;
		while (m_StrData[index] != '\0' && string[index] != '\0')
		{
			if (m_StrData[index] != string[index])
			{
				std::cout << m_StrData[index] << ", " << string[index];
				return false;
			}
			index++;
		}
		return (m_StrData[index] == string[index]);
	}

	bool String::equals(const String& string) const
	{
		return equals(string.m_StrData);
	}
	
}