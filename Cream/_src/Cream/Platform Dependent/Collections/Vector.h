#pragma once
#include "Cream/Core/Assertions.h"

namespace Cream
{
	template<typename T>
	class Vector
	{
	public:
		class Iterator;

		Vector()
		{
			// Allocate 2 elements
			realloc(10);
		}

		~Vector()
		{
			clear();
			::operator delete(m_Data, m_Capacity * sizeof(T));
		}

		void pushBack(const T& value)
		{
			if (m_Size >= m_Capacity)
			{
				realloc(m_Capacity + m_Capacity / 2);
			}

			m_Data[m_Size] = value;
			m_Size++;
		}

		void pushBack(T&& value)
		{
			if (m_Size >= m_Capacity)
			{
				realloc(m_Capacity + m_Capacity / 2);
			}

			m_Data[m_Size] = std::move(value);
			m_Size++;
		}

		template<typename... Args>
		T& emplaceBack(Args&&... args)
		{
			if (m_Size >= m_Capacity)
				realloc(m_Capacity + m_Capacity / 2);

			new(&m_Data[m_Size]) T(std::forward<Args>(args)...);
			return m_Data[m_Size++];
		}

		void popBack()
		{
			if (m_Size > 0)
			{
				m_Size--;
				m_Data[m_Size].~T();
			}
		}

		void popAt(const Iterator& index)
		{
			popAt(index.getIndex());	
		}

		void popAt(const size_t& index)
		{
			CREAM_ASSERT(index >= 0 && index < m_Size);
			m_Data[i].~T();
			for (size_t i = index; i < m_Size - 1; i++)
			{
				m_Data[i] = std::move(m_Data[i + 1]);
				m_Data[i+1].~T();
			}
			m_Size--;
		}

		void popAt(const Iterator& first, const Iterator& last)
		{
			popAt(first.getIndex(), last.getIndex());
		}

		void popAt(const size_t& first, const size_t& last)
		{
			CREAM_ASSERT(first >= 0 && first <= last && last < m_Size);
			for (size_t i = first; i <= last; i++)
			{
				m_Data[i].~T();
			}

			for (size_t i = 0; i < m_Size - last - 1; i++)
			{
				m_Data[first + i] = std::move(m_Data[last + i + 1]);
				m_Data[last + i + 1].~T();
			}
			m_Size -= last - first + 1;
		}

		void clear()
		{
			for (size_t i = 0; i < m_Size; i++)
				m_Data[i].~T();

			m_Size = 0;
		}

		void shrinkToFit()
		{
			realloc(m_Size);
		}

		void resize(const size_t newSize)
		{
			realloc(newSize);
		}

		void reserve(const size_t newCapacity)
		{
			if (newCapacity > m_Capacity)
				resize(newCapacity);
		}

		const T& operator[](size_t index) const
		{
			CREAM_ASSERT(index < m_Size);
			return m_Data[index];
		}

		T& operator[](size_t index)
		{
			CREAM_ASSERT(index < m_Size);
			return m_Data[index];
		}

		const T& operator[](const Iterator& iterator) const
		{
			return m_Data[iterator.getIndex()];
		}

		T& operator[](const Iterator& iterator)
		{
			return m_Data[iterator.getIndex()];
		}

		size_t getSize() const { return m_Size; }
		size_t getCapacity() const { return m_Capacity; }
		
		const Iterator& begin() const { return Iterator(m_Data, 0); }
		const Iterator& end() const { return Iterator(m_Data, m_Size); }

		class Iterator
		{
		public:
			Iterator(T* data, size_t index)
				: m_Data(data), m_Index(index)
			{
			}

			const size_t& getIndex() const
			{
				return m_Index;
			}

			const T& operator*() const
			{
				return m_Data[m_Index];
			}

			T& operator*()
			{
				return m_Data[m_Index];
			}

			Iterator& operator++()
			{
				m_Index++;
				return *this;
			}

			Iterator operator++(int)
			{
				Iterator iterator = *this;
				++* this;
				return iterator;
			}

			void operator+=(size_t num)
			{
				m_Index += num;
			}

			bool operator!=(const Iterator& other)
			{
				return m_Index != other.m_Index;
			}

		private:
			T* m_Data;
			size_t m_Index = 0;
		};

	private:
		void realloc(size_t newCapacity)
		{
			T* newBlock = (T*)::operator new(newCapacity * sizeof(T));

			if (newCapacity < m_Size)
				m_Size = newCapacity;

			for (size_t i = 0; i < m_Size; i++)
				newBlock[i] = std::move(m_Data[i]);

			for (size_t i = 0; i < m_Size; i++)
				m_Data[i].~T();

			::operator delete(m_Data, m_Capacity * sizeof(T));
			m_Data = newBlock;
			m_Capacity = newCapacity;
		}

	private:
		T* m_Data = nullptr;
		size_t m_Size = 0;
		size_t m_Capacity = 0;
	};
}