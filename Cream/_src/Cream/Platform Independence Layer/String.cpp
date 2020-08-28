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

	void String::cpy(char* const dest, const char* const src, const size_t offset, const size_t& length)
	{
		memcpy(dest, &src[offset], length);
		dest[length] = '\0';
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
		memcpy(&dest[globalIndex], src2.m_Buffer, src2.m_Length + 1);
	}

	void String::cat(char* const dest, const String& src1, const char* const src2)
	{
		if (!dest)
			return;

		memcpy(dest, src1.m_Buffer, src1.m_Length);

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
		memcpy(dest, src1.m_Buffer, src1.m_Length);
		memcpy(&dest[src1.m_Length], src2.m_Buffer, src2.m_Length+1);
	}

	String::String(const size_t& capacity)
		: m_Buffer(nullptr), m_Length(0)
	{
		m_Buffer = allocate(capacity);
		m_Buffer[0] = '\0';
	}

	String::String(const char& string, const size_t& capacity)
		: m_Length(1)
	{
		CREAM_ASSERT(capacity >= 1);
		m_Buffer = allocate(capacity);
		m_Buffer[0] = string;
		m_Buffer[1] = '\0';
	}

	String::String(const char* const string)
	{
		if (string == nullptr)
		{
			m_Length = 0;
			m_Buffer = allocate(0);
			m_Buffer[0] = '\0';
		}
		else
		{
			m_Length = len(string);
			m_Buffer = allocate(m_Length);
			cpy(m_Buffer, string);
		}
	}

	String::String(const char* const string, const size_t& capacity)
	{
		if (string == nullptr)
		{
			m_Length = 0;
			m_Buffer = allocate(capacity);
			m_Buffer[0] = '\0';
		}
		else
		{
			m_Length = len(string);
			CREAM_ASSERT(capacity >= m_Length);
			m_Buffer = allocate(capacity);
			cpy(m_Buffer, string);
		}
	}

	String::String(const char* const string, const size_t& offset, const size_t& length)
	{
		if (string == nullptr)
		{
			m_Length = 0;
			m_Buffer = allocate(0);
			m_Buffer[0] = '\0';
		}
		else
		{
			m_Length = length;
			m_Buffer = allocate(m_Length);
			cpy(m_Buffer, string, offset, length);
		}
	}

	String::String(const char* const string, const size_t& offset, const size_t& length, const size_t& capacity)
	{
		if (string == nullptr)
		{
			m_Length = 0;
			m_Buffer = allocate(capacity);
			m_Buffer[0] = '\0';
		}
		else
		{
			m_Length = length;
			CREAM_ASSERT(capacity >= m_Length);
			m_Buffer = allocate(capacity);
			cpy(m_Buffer, string, offset, length);
		}
	}

	String::String(const String& otherStr)
		: m_Length(otherStr.m_Length)
	{
		m_Length = otherStr.m_Length;
		m_Buffer = allocate(m_Length);
		cpy(m_Buffer, otherStr.m_Buffer, 0, otherStr.m_Length);
	}

	String::String(const String& otherStr, const size_t& capacity)
		: m_Length(otherStr.m_Length)
	{
		m_Length = otherStr.m_Length;
		CREAM_ASSERT(capacity >= m_Length);
		m_Buffer = allocate(capacity);
		cpy(m_Buffer, otherStr.m_Buffer, 0, otherStr.m_Length);
	}

	String::String(String&& otherStr) noexcept
		: m_Length(otherStr.m_Length)
	{
		m_Buffer = otherStr.m_Buffer;
		m_Capacity = otherStr.m_Capacity;
		otherStr.m_Buffer = nullptr;
	}

	String& String::operator=(const char& string)
	{
		if (m_Capacity < 1)
		{
			delete m_Buffer;
			m_Buffer = allocate(1);
		}
		m_Length = 1;
		m_Buffer[0] = string;
		m_Buffer[1] = '\0';
		return *this;
	}
	String& String::operator=(const char* const string)
	{
		if (string == nullptr)
		{
			m_Length = 0;
			m_Buffer[0] = '\0';
		}
		else
		{
			m_Length = len(string);
			if (m_Length > m_Capacity)
			{
				delete m_Buffer;
				m_Buffer = allocate(m_Length);
			}
			cpy(m_Buffer, string, 0, m_Length);
		}
		return *this;
	}
	String& String::operator=(const String& otherStr)
	{
		if (this == &otherStr)
			return *this;

		if (m_Capacity < otherStr.m_Length)
		{
			delete m_Buffer;
			m_Buffer = allocate(otherStr.m_Length);
		}
		m_Length = otherStr.m_Length;
		cpy(m_Buffer, otherStr.m_Buffer, 0, otherStr.m_Length);

		return *this;
	}
	String& String::operator=(String&& otherStr) noexcept
	{
		delete m_Buffer;

		m_Length = otherStr.m_Length;
		m_Capacity = otherStr.m_Capacity;
		m_Buffer = otherStr.m_Buffer;
		otherStr.m_Buffer = nullptr;

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
		String outStr = String(m_Length + 1);
		outStr.m_Length = m_Length + 1;
		memcpy(outStr.m_Buffer, m_Buffer, m_Length);
		outStr.m_Buffer[outStr.m_Length - 1] = string;
		outStr.m_Buffer[outStr.m_Length] = '\0';
		return outStr;
	}
	String String::operator+(const char* const string)
	{
		const size_t newLength = m_Length + len(string);
		String outStr = String(newLength);
		outStr.m_Length = newLength;
		cat(outStr.m_Buffer, *this, string);
		return outStr;
	}
	String String::operator+(const String& otherStr)
	{
		String outStr = String(m_Length + otherStr.m_Length);
		outStr.m_Length = m_Length + otherStr.m_Length;
		cat(outStr.m_Buffer, *this, otherStr);
		return outStr;
	}
	String operator+(const char& string, const String& otherStr)
	{
		String outStr = String(otherStr.m_Length + 1);
		outStr.m_Length = otherStr.m_Length + 1;
		outStr.m_Buffer[0] = string;
		String::cpy(&outStr.m_Buffer[1], otherStr.m_Buffer, 0, otherStr.m_Length);
		return outStr;
	}
	String operator+(const char* const string, const String& otherStr)
	{
		const size_t newLength = String::len(string) + otherStr.m_Length;
		String outStr = String(newLength);
		outStr.m_Length = newLength;
		String::cat(outStr.m_Buffer, string, otherStr);
		return outStr;
	}

	char& String::operator[](const size_t& index)
	{
		return m_Buffer[index];
	}

	bool String::operator==(const char* const string)
	{
		return equals(string);
	}

	bool String::operator==(const String& string)
	{
		return equals(string.m_Buffer);
	}

	bool String::operator!=(const char* const string)
	{
		return !equals(string);;
	}

	bool String::operator!=(const String& string)
	{
		return !equals(string.m_Buffer);;
	}

	void String::reserve(const size_t& characters)
	{
		if (characters < m_Capacity)
			return;

		char* newBuffer = allocate(characters);
		cpy(newBuffer, m_Buffer, 0, m_Length);
		delete m_Buffer;
		m_Buffer = newBuffer;
	}

	void String::shrink(const size_t& characters)
	{
		CREAM_ASSERT(characters >= 0);
		char* newBuffer = allocate(characters);
		const size_t copyLength = (characters < m_Length) ? characters : m_Length;
		memcpy(newBuffer, m_Buffer, copyLength);
		newBuffer[copyLength] = '\0';
		delete m_Buffer;
		m_Buffer = newBuffer;
	}

	void String::shrinkToFit()
	{
		if (m_Capacity == m_Length)
			return;

		char* newBuffer = allocate(m_Length);
		cpy(newBuffer, m_Buffer, 0, m_Length);
		delete m_Buffer;
		m_Buffer = newBuffer;
	}

	void String::pushBack(const char& string)
	{
		size_t newLength = m_Length + 1;
		char* newBuffer = m_Buffer;
		if (newLength > m_Capacity)
		{
			newBuffer = allocate(newLength);
			cpy(newBuffer, m_Buffer, 0, m_Length);
			delete m_Buffer;
		}
		newBuffer[newLength - 1] = string;
		newBuffer[newLength] = '\0';
		m_Buffer = newBuffer;
		m_Length = newLength;
	}
	void String::pushBack(const char* const string)
	{
		const size_t otherLength = len(string);
		const size_t newLength = m_Length + otherLength;
		if (newLength > m_Capacity)
		{
			char* newBuffer = allocate(newLength);
			cat(newBuffer, *this, string);
			delete m_Buffer;
			m_Buffer = newBuffer;
		}
		else
		{
			memcpy(&m_Buffer[m_Length], string, otherLength + 1);
		}
		m_Length = newLength;
	}
	void String::pushBack(const String& otherStr)
	{
		const size_t newLength = m_Length + otherStr.m_Length;
		if (newLength > m_Capacity)
		{
			char* newBuffer = allocate(newLength);
			cat(newBuffer, *this, otherStr);
			delete m_Buffer;
			m_Buffer = newBuffer;
		}
		else
		{
			memcpy(&m_Buffer[m_Length], otherStr.data(), otherStr.m_Length + 1);
		}

		m_Length = newLength;
	}
	void String::pushFront(const char* const string)
	{
		const size_t otherLength = len(string);
		const size_t newLength = m_Length + otherLength;
		if (newLength > m_Capacity)
		{
			char* newBuffer = new char[newLength + 1];
			cat(newBuffer, string, *this);
			delete m_Buffer;
			m_Buffer = newBuffer;
		}
		else
		{
			memmove(&m_Buffer[otherLength], m_Buffer, m_Length + 1);
			memcpy(m_Buffer, string, otherLength);
		}
		m_Length = newLength;
	}
	void String::pushFront(const String& otherStr)
	{
		const size_t newLength = m_Length + otherStr.m_Length;
		if (newLength > m_Capacity)
		{
			char* newBuffer = new char[newLength + 1];
			cat(newBuffer, otherStr, *this);
			delete m_Buffer;
			m_Buffer = newBuffer;
		}
		else
		{
			memmove(&m_Buffer[otherStr.m_Length], m_Buffer, m_Length + 1);
			memcpy(m_Buffer, otherStr.m_Buffer, otherStr.m_Length);
		}
		m_Length = newLength;
	}

	String String::substr(const size_t& first, const size_t& last)
	{
		CREAM_ASSERT(first >= 0 && first <= last && last <= m_Length);
		String outStr = String(last - first);
		cpy(outStr.m_Buffer, m_Buffer, first, last - first);
		outStr.m_Length = last - first;
		return outStr;
	}

	String String::substr(const size_t& first, const Iterator& last)
	{
		return substr(first, last.m_Index);
	}

	String String::substr(const Iterator& first, const size_t& last)
	{
		return substr(first.m_Index, last);
	}

	String String::substr(const Iterator& first, const Iterator& last)
	{
		return substr(first.m_Index, last.m_Index);
	}

	String& String::replace(char targetVal, char newVal)
	{
		for (size_t i = 0; i < m_Length; i++)
		{
			if (m_Buffer[i] == targetVal) m_Buffer[i] = newVal;
		}
		return *this;
	}

	String& String::replace(const char* const targetVal, const char* const newVal)
	{
		const size_t targetLength = len(targetVal);
		const size_t newLength = len(newVal);
		size_t newTotalLength = 0;
		char* newBuffer = m_Buffer;

		size_t targetInstances = 0;
		for (size_t i = 0; i + targetLength - 1 < m_Length; i++)
		{
			bool isInstance = true;
			for (size_t t = 0; t < targetLength; t++)
			{
				if (m_Buffer[i + t] != targetVal[t])
				{
					isInstance = false;
					break;
				}
			}
			if (isInstance)
			{
				targetInstances += 1;
				i += targetLength - 1;
			}
		}
		newTotalLength = m_Length + targetInstances * (newLength - targetLength);
		size_t globalIndex = 0;
		if (newTotalLength > m_Capacity)
		{
			newBuffer = allocate(newTotalLength);
			for (size_t i = 0; i < m_Length; i++)
			{
				if (i + targetLength - 1 < m_Length)
				{
					bool isInstance = true;
					for (size_t t = 0; t < targetLength; t++)
					{
						if (m_Buffer[i + t] != targetVal[t])
						{
							isInstance = false;
							break;
						}
					}
					if (isInstance)
					{
						memcpy(&newBuffer[globalIndex], newVal, newLength);
						i += targetLength - 1;
						globalIndex += newLength;
					}
					else
					{
						newBuffer[globalIndex] = m_Buffer[i];
						globalIndex++;
					}
				}	
				else
				{
					newBuffer[globalIndex] = m_Buffer[i];
					globalIndex++;
				}
			}
			
			delete m_Buffer;
			m_Buffer = newBuffer;
			m_Length = newTotalLength;
		}
		else
		{
			for (size_t i = 0; i < m_Length; i++)
			{
				if (i + targetLength - 1 < m_Length)
				{
					bool isInstance = true;
					for (size_t t = 0; t < targetLength; t++)
					{
						if (m_Buffer[i + t] != targetVal[t])
						{
							isInstance = false;
							break;
						}
					}
					if (isInstance)
					{
						memcpy(&newBuffer[globalIndex], newVal, newLength);
						i += targetLength - 1;
						globalIndex += newLength;
					}
					else
					{
						newBuffer[globalIndex] = m_Buffer[i];
						globalIndex++;
					}
				}
				else
				{
					newBuffer[globalIndex] = m_Buffer[i];
					globalIndex++;
				}
			}
		}
		newBuffer[newTotalLength] = '\0';
		return *this;
	}

	void String::clear()
	{
		m_Buffer[0] = '\0';
		m_Length = 0;
	}

	bool String::equals(const char* const string) const
	{
		size_t index = 0;
		while (m_Buffer[index] != '\0' && string[index] != '\0')
		{
			if (m_Buffer[index] != string[index])
			{
				std::cout << m_Buffer[index] << ", " << string[index];
				return false;
			}
			index++;
		}
		return (m_Buffer[index] == string[index]);
	}

	bool String::equals(const String& string) const
	{
		return equals(string.m_Buffer);
	}

	bool String::empty() const	
	{ 
		return m_Buffer[0] == '\0';
	}

	char* String::allocate(const size_t& characters)
	{
		m_Capacity = characters;
		char* newBufferPtr = new char[characters + 1];
		CREAM_ASSERT(newBufferPtr);
		return newBufferPtr;
	}

	String::Iterator String::begin() const
	{
		return Iterator(&m_Buffer, 0);
	}

	String::Iterator String::end() const
	{
		return Iterator(&m_Buffer, m_Length);
	}

	String::Iterator String::rbegin() const
	{
		return Iterator(&m_Buffer, m_Length-1);
	}

	String::Iterator String::rend() const
	{
		return Iterator(&m_Buffer, -1);
	}

	String::Iterator::Iterator(char*const* buffer, const size_t& index)
		: m_Buffer(buffer), m_Index(index)
	{
	}

	char& String::Iterator::operator*() const
	{
		return m_Buffer[0][m_Index];
	}

	bool String::Iterator::operator!=(const Iterator& other) const
	{
		return m_Index != other.m_Index;
	}

	String::Iterator& String::Iterator::operator++()
	{
		m_Index++;
		return *this;
	}

	String::Iterator String::Iterator::operator++(int)
	{
		Iterator iterator = *this;
		++* this;
		return iterator;
	}


	String::Iterator& String::Iterator::operator--()
	{
		m_Index--;
		return *this;
	}

	String::Iterator String::Iterator::operator--(int)
	{
		Iterator iterator = *this;
		--* this;
		return iterator;
	}

}