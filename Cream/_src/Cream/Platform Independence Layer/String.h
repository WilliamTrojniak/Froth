#pragma once

namespace Cream
{
	class String
	{
	public:
		class Iterator;

		static size_t len(const char* const str);
		static void cpy(char* const dest, const char* const src);
		static void cpy(char* const dest, const char* const src, const size_t offset, const size_t& length);
		static void cat(char* const dest, const char* const src1, const char* const src2);
		static void cat(char* const dest, const char* const src1, const String& src2);
		static void cat(char* const dest, const String& src1, const char* const src2);
		static void cat(char* const dest, const String& src1, const String& src2);
		
		String(const size_t& capacity = 0);
		String(const char& string, const size_t& capacity = 1);
		String(const char* const string);
		String(const char* const string, const size_t& capacity);
		String(const char* const string, const size_t& offset, const size_t& length);
		String(const char* const string, const size_t& offset, const size_t& length, const size_t& capacity);
		String(const String& otherStr);
		String(const String& otherStr, const size_t& capacity);
		String(String&& otherStr) noexcept;
		~String() { delete m_Buffer; }

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

		// Grows the size of the string buffer to the desired number of characters
		void reserve(const size_t& characters);
		// Sets the size of the string buffer to the desired number of characters
		void shrink(const size_t& characters);
		// Sets the size of the string buffer to the length of the currently stored string
		void shrinkToFit();

		void pushBack(const char& string);
		void pushBack(const char* const string);
		void pushBack(const String& otherStr);
		void pushFront(const char* const string);
		void pushFront(const String& otherStr);

		String substr(const size_t& first, const size_t& last);// returns a substring of string with a value of string[first, last)
		String substr(const size_t& first, const Iterator& last);
		String substr(const Iterator& first, const size_t& last);
		String substr(const Iterator& first, const Iterator& last);
		String& replace(char targetVal, char newVal);
		String& replace(const char* const targetVal, const char* const newVal);

		void clear(); // Clears the string buffer by setting the length to 0 and value at index 0 of the buffer to '\0'

		bool equals(const char* const string) const;
		bool equals(const String& string) const;
		bool empty() const;

		char* data() { return m_Buffer; }
		const char* data() const { return m_Buffer; }
		const size_t& length() const { return m_Length; }
		const size_t& capacity() const { return m_Capacity; }

	private:
		char* allocate(const size_t& characters); // Characeters refers to useable characters

		size_t m_Capacity = 0; // Does not include '\0' character
		size_t m_Length = 0; // Does not include '\0' character
		char* m_Buffer = nullptr;

	public:
		Iterator begin() const;
		Iterator end() const;
		Iterator rbegin() const;
		Iterator rend() const;

		class Iterator
		{
		public:
			friend class String;

			bool operator!=(const Iterator& other) const;
			char& operator*() const;
			Iterator& operator++();
			Iterator operator++(int);
			Iterator& operator--();
			Iterator operator--(int);
		private:
			Iterator(char*const* buffer, const size_t& index);
			
			char* const* m_Buffer = nullptr;
			size_t m_Index = 0;
		};
	};
}