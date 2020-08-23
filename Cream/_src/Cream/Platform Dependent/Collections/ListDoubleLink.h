#pragma once

namespace Cream
{
	template <typename T>
	class ListDouble
	{
	private:
		class Node;
		
	public:
		class Iterator;

		ListDouble()
			: m_Head(nullptr), m_Tail(nullptr)
		{
		}

		~ListDouble()
		{
			clear();
		}

		void pushFront(const T& value)
		{
			Node* newNode = new Node(value, nullptr, m_Head);
			if (m_Head == nullptr)
			{
				m_Tail = newNode;
			}
			else
			{
				m_Head->setPrev(newNode);
			}
			m_Head = newNode;
		}

		void pushFront(T&& value)
		{
			Node* newNode = new Node(std::move(value), nullptr, m_Head);
			if (m_Head == nullptr)
			{
				m_Tail = newNode;
			}
			else
			{
				m_Head->setPrev(newNode);
			}
			m_Head = newNode;
		}

		template<typename... Args>
		void emplaceFront(Args&&... args)
		{
			Node* newNode = new Node(nullptr, m_Head, std::forward<Args>(args)...);
			if (m_Head == nullptr)
			{
				m_Tail = newNode;
			}
			else
			{
				m_Head->setPrev(newNode);
			}
			m_Head = newNode;
		}

		void pushBack(const T& value)
		{
			Node* newNode = new Node(value, m_Tail, nullptr);
			if (m_Head == nullptr)
			{
				m_Head = newNode;
			}
			else
			{
				m_Tail->setNext(newNode);
			}
			m_Tail = newNode;
		}

		void pushBack(T&& value)
		{
			Node* newNode = new Node(std::move(value), m_Tail, nullptr);
			if (m_Head == nullptr)
			{
				m_Head = newNode;
			}
			else
			{
				m_Tail->setNext(newNode);
			}
			m_Tail = newNode;
		}

		template<typename... Args>
		void emplaceBack(Args&&... args)
		{
			Node* newNode = new Node(m_Tail, nullptr, std::forward<Args>(args)...);
			if (m_Head == nullptr)
			{
				m_Head = newNode;
			}
			else
			{
				m_Tail->setNext(newNode);
			}
			m_Tail = newNode;
		}

		// Assumes list is of a size greater than or equal to 1
		void pushAfter(const T& value, const Iterator& index)
		{
			Node* newNode = new Node(value, index.getCurrentNode(), index.getCurrentNode()->getNext());
			if (index.getCurrentNode()->getNext())
				index.getCurrentNode()->getNext()->setPrev(newNode);
			index.getCurrentNode()->setNext(newNode);
			if (index.getCurrentNode() == m_Tail)
				m_Tail = newNode;
		}

		void pushAfter(T&& value, const Iterator& index)
		{
			Node* newNode = new Node(std::move(value), index.getCurrentNode(), index.getCurrentNode()->getNext());
			if (index.getCurrentNode()->getNext())
				index.getCurrentNode()->getNext()->setPrev(newNode);
			index.getCurrentNode()->setNext(newNode);
			if (index.getCurrentNode() == m_Tail)
				m_Tail = newNode;
		}

		template<typename...Args>
		void emplaceAfter(const Iterator& index, Args&&... args)
		{
			Node* newNode = new Node(index.getCurrentNode(), index.getCurrentNode()->getNext(), std::forward<Args>(args)...);
			if (index.getCurrentNode()->getNext())
				index.getCurrentNode()->getNext()->setPrev(newNode);
			index.getCurrentNode()->setNext(newNode);
			if (index.getCurrentNode() == m_Tail)
				m_Tail = newNode;
		}

		// Assumes list is of a size greater than or equal to 1
		void pushBefore(const T& value, const Iterator& index)
		{
			Node* newNode = new Node(value, index.getCurrentNode()->getPrev(), index.getCurrentNode());
			if (index.getCurrentNode()->getPrev())
				index.getCurrentNode()->getPrev()->setNext(newNode);
			index.getCurrentNode()->setPrev(newNode);
			if (index.getCurrentNode() == m_Head)
				m_Head = newNode;
		}

		void pushBefore(T&& value, const Iterator& index)
		{
			Node* newNode = new Node(std::move(value), index.getCurrentNode()->getPrev(), index.getCurrentNode());
			if (index.getCurrentNode()->getPrev())
				index.getCurrentNode()->getPrev()->setNext(newNode);
			index.getCurrentNode()->setPrev(newNode);
			if (index.getCurrentNode() == m_Head)
				m_Head = newNode;
		}

		template<typename...Args>
		void emplaceBefore(const Iterator& index, Args&&... args)
		{
			Node* newNode = new Node(index.getCurrentNode()->getPrev(), index.getCurrentNode(), std::forward<Args>(args)...);
			if (index.getCurrentNode()->getPrev())
				index.getCurrentNode()->getPrev()->setNext(newNode);
			index.getCurrentNode()->setPrev(newNode);
			if (index.getCurrentNode() == m_Head)
				m_Head = newNode;
		}
		
		void clear()
		{
			Node* nextNode = m_Head;
			Node* currentNode = m_Head;
			while (nextNode)
			{
				currentNode = nextNode;
				nextNode = currentNode->getNext();
				delete currentNode;
			}
			m_Head = nullptr;
			m_Tail = nullptr;
		}

		void popFront()
		{
			if (m_Head)
			{
				if (m_Head->getNext())
				{
					m_Head = m_Head->getNext();
					delete m_Head->getPrev();
					m_Head->setPrev(nullptr);
					return;
				}

				delete m_Head;
				m_Head = nullptr;
				m_Tail = nullptr;
			}
		}

		void popBack()
		{
			if (m_Tail)
			{
				if (m_Tail->getPrev())
				{
					m_Tail = m_Tail->getPrev();
					delete m_Tail->getNext();
					m_Tail->setNext(nullptr);
					return;
				}
				delete m_Tail;
				m_Head = nullptr;
				m_Tail = nullptr;
			}
		}

		void popAt(const Iterator& index)
		{
			if (index.getCurrentNode()->getPrev())
			{
				index.getCurrentNode()->getPrev()->setNext(index.getCurrentNode()->getNext());
			}
			else
			{
				m_Head = index.getCurrentNode()->getNext();
			}

			if (index.getCurrentNode()->getNext())
			{
				index.getCurrentNode()->getNext()->setPrev(index.getCurrentNode()->getPrev());
			}
			else
			{
				m_Tail = index.getCurrentNode()->getPrev();
			}
			delete index.getCurrentNode();
		}

		void remove(const T& value)
		{
			Iterator i = search(value);
			if (i.getCurrentNode())
			{
				popAt(i);
			}
		}

		Iterator& search(const T& value) const
		{
			for (Iterator i = begin(); i != end(); i++)
			{
				if (i.getCurrentNode()->getData() == value)
					return i;
			}
			return end();
		}

		bool isEmpty() const
		{
			return m_Head == nullptr;
		}

		Iterator& begin()
		{
			return Iterator(m_Head);
		}

		const Iterator& begin() const
		{
			return Iterator(m_Head);
		}

		Iterator& end()
		{
			return Iterator(nullptr);
		}

		const Iterator& end() const
		{
			return Iterator(nullptr);
		}

		Iterator& rbegin() 
		{
			return Iterator(m_Tail);
		}

		const Iterator& rbegin() const
		{
			return Iterator(m_Tail);
		}

		const Iterator& rend() const
		{
			return Iterator(nullptr);
		}

		Iterator& rend()
		{
			return Iterator(nullptr);
		}

		T& operator[](const Iterator& iterator)
		{
			return *iterator;
		}

		const T& operator[](const Iterator& iterator) const
		{
			return *iterator;
		}

		class Iterator
		{
		public:
			Iterator(Node* nodeAddr)
				: m_CurrentNode(nodeAddr)
			{
			}

			Node* getCurrentNode() const
			{
				return m_CurrentNode;
			}

			Iterator& operator=(Node* node)
			{
				m_CurrentNode = node;
				return *this;
			}

			Iterator& operator++()
			{
				if (m_CurrentNode)
					m_CurrentNode = m_CurrentNode->getNext();
				return *this;
			}

			Iterator operator++(int)
			{
				Iterator iterator = *this;
				++* this;
				return iterator;
			}

			Iterator& operator--()
			{
				if (m_CurrentNode)
					m_CurrentNode = m_CurrentNode->getPrev();
				return *this;
			}

			Iterator operator--(int)
			{
				Iterator iterator = *this;
				--* this;
				return iterator;
			}

			bool operator!=(const Iterator& iterator) const
			{
				return m_CurrentNode != iterator.m_CurrentNode;
			}

			bool operator==(const Iterator& iterator) const
			{
				return m_CurrentNode == iterator.m_CurrentNode;
			}

			const T& operator*() const
			{
				return m_CurrentNode->getData();
			}

			T& operator*()
			{
				return m_CurrentNode->getData();
			}

		private:
			Node* m_CurrentNode;
		};

	private:
		Node* m_Head;
		Node* m_Tail;

		class Node
		{
		public:
			Node(const T& data, Node* prevNode, Node* nextNode)
				: m_Data(data), m_PrevNode(prevNode), m_NextNode(nextNode)
			{
			}

			Node(T&& data, Node* prevNode, Node* nextNode)
				: m_Data(std::move(data)), m_PrevNode(prevNode), m_NextNode(nextNode)
			{
			}

			template<typename... Args>
			Node(Node* prevNode, Node* nextNode, Args&&... args)
				: m_PrevNode(prevNode), m_NextNode(nextNode), m_Data(T(std::forward<Args>(args)...))
			{
			}

			T& getData()
			{
				return m_Data;
			}

			const T& getData() const
			{
				return m_Data;
			}

			Node* getPrev() const
			{
				return m_PrevNode;
			}

			Node* getNext() const
			{
				return m_NextNode;
			}

			void setPrev(Node* prevNode)
			{
				m_PrevNode = prevNode;
			}
			void setNext(Node* nextNode)
			{
				m_NextNode = nextNode;
			}

		private:
			T m_Data;
			Node* m_PrevNode;
			Node* m_NextNode;
		};
	};
}