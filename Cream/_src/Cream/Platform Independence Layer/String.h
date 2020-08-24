#pragma once

namespace Cream
{
	class String
	{
	public:
		static size_t len(const char* const str);
		static void cpy(char* const dest, const char* const src);
		static void cpy(char* const dest, const String& src);
		static void cat(char* const dest, const char* const src1, const char* const src2);
		static void cat(char* const dest, const char* const src1, const String& src2);
		static void cat(char* const dest, const String& src1, const char* const src2);
		static void cat(char* const dest, const String& src1, const String& src2);
		
		String();
		String(const char& string);
		String(const char* const string);
		String(const String& otherStr);
		String(String&& otherStr) noexcept;
		~String() { delete m_StrData; }

		String& operator=(const char& string);
		String& operator=(const char* const string);
		String& operator=(const String& otherStr);
		String& operator=(String&& otherStr) noexcept;
		String& operator+=(const char& string);
		String& operator+=(const char* const string);
		String& operator+=(const String& otherStr);
		String operator+(const char& string);
		String operator+(const char* const string);
		String operator+(const String& otherStr);
		friend String operator+(const char& string, const String& otherStr);
		friend String operator+(const char* const string, const String& otherStr);
		char& operator[](const size_t& index);
		bool operator==(const char* const string);
		bool operator==(const String& string);
		bool operator!=(const char* const string);
		bool operator!=(const String& string);

		void pushBack(const char& string);
		void pushBack(const char* const string);
		void pushBack(const String& otherStr);
		void pushFront(const char* const string);
		void pushFront(const String& otherStr);

		char* data() const { return m_StrData; }
		const size_t& length() const { return m_Length; }
		bool equals(const char* const string) const;
		bool equals(const String& string) const;

	private:
		size_t m_Capacity;
		size_t m_Length;
		char* m_StrData;
	};
}