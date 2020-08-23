#pragma once

namespace Cream
{
	template <typename T>
	class List
	{
	private:
		class Node;

	public:
		// Forward declaration of the iterator class for the list
		class Iterator;

		// Creates a list of size 0
		List()
			: m_Head(nullptr), m_Tail(nullptr)
		{

		}
		~List()
		{
			clear();
		}


		// Inserts node with data of type 'T' to front of list
		void pushFront(const T& value)
		{
			Node* newNode = new Node(value);
			newNode->setNext(m_Head);

			if (m_Head == nullptr)
			{
				m_Tail = newNode;
			}

			m_Head = newNode;
		}

		void pushFront(T&& value)
		{
			Node* newNode = new Node(std::move(value));
			newNode->setNext(m_Head);

			if (m_Head == nullptr)
			{
				m_Tail = newNode;
			}

			m_Head = newNode;
		}

		template<typename...Args>
		void emplaceFront(Args&&... args)
		{
			Node* newNode = new Node(std::forward<Args>(args)...);
			newNode->setNext(m_Head);

			if (m_Head == nullptr)
			{
				m_Tail = newNode;
			}

			m_Head = newNode;
		}

		// Appends node with data of type 'T' to end of list
		void pushBack(const T& value)
		{
			Node* newNode = new Node(value);
			
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
			Node* newNode = new Node(std::move(value));

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

		template<typename...Args>
		void emplaceBack(Args&&... args)
		{
			Node* newNode = new Node(std::forward<Args>(args)...);

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

		// Inserts node with data of type 'T' after specified node
		void pushAfter(const T& value, const Iterator& index)
		{
			Node* newNode = new Node(value);
			newNode->setNext(index.getCurrentNode()->getNext());
			index.getCurrentNode()->setNext(newNode);
			if (index.getCurrentNode() == m_Tail)
				m_Tail = newNode;
			
		}

		void pushAfter(T&& value, const Iterator& index)
		{
			Node* newNode = new Node(std::move(value));
			newNode->setNext(index.getCurrentNode()->getNext());
			index.getCurrentNode()->setNext(newNode);
			if (index.getCurrentNode() == m_Tail)
				m_Tail = newNode;

		}

		template<typename...Args>
		void emplaceAfter(const Iterator& index, Args&&... args)
		{
			Node* newNode = new Node(std::forward<Args>(args)...);
			newNode->setNext(index.getCurrentNode()->getNext());
			index.getCurrentNode()->setNext(newNode);
			if (index.getCurrentNode() == m_Tail)
				m_Tail = newNode;
		}

		// Deletes the node at the front of the list
		void popFront()
		{
			if (m_Head != nullptr)
			{
				Node* newHead = m_Head->getNext();
				delete m_Head;
				m_Head = newHead;
			}
		}

		// Deletes the node at the back of the list
		// CAUTION: Must loop through entire list to determine second last node
		// thus runtime of O(n)
		void popBack()
		{
			if (m_Head == m_Tail)
			{
				m_Head = nullptr;
				m_Tail = nullptr;
				return;
			}

			Node* prevNode = this->begin().getCurrentNode();
			for (Iterator i = this->begin()++; i != this->end(); i++)
			{
				if (i.getCurrentNode() == this->m_Tail)
				{
					delete m_Tail;
					prevNode->setNext(nullptr);
					m_Tail = prevNode;
					return;
				}
				prevNode = i.getCurrentNode();
			}
		}

		// Delete at a given node index
		// CAUTION: Runtime of O(n) due to search for node that preceeds node requesting deletion
		void popAt(const Iterator& index)
		{
			if (index.getCurrentNode() == nullptr)
				return;

			if (index == this->begin())
			{
				popFront();
				return;
			}

			if (index == this->end())
			{
				popBack();
				return;
			}

			Node* prevNode = this->begin().getCurrentNode();
			for (Iterator i = this->begin()++; i != this->end(); i++)
			{
				if (i.getCurrentNode() == index.getCurrentNode())
				{
					prevNode->setNext(i.getCurrentNode()->getNext());
					delete i.getCurrentNode();
					return;
				}
				prevNode = i.getCurrentNode();
			}
		}

		// Frees all the memory allocated by the nodes that make up the list
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

		// Removes the first node found with a given value
		// CAUTION: Runtime of O(n) needed to loop through all values and find previous node
		void remove(const T& value)
		{
			if (this->isEmpty())
				return;

			Node* prevNode = this->begin().getCurrentNode();
			for (Iterator i = this->begin(); i != this->end(); i++)
			{
				if (i.getCurrentNode()->getData() == value)
				{
					if (i == this->begin())
					{
						return this->popFront();
					}

					if (i.getCurrentNode() == m_Tail)
					{
						return this->popBack();
					}

					prevNode->setNext(i.getCurrentNode()->getNext());
					delete i.getCurrentNode();
					return;
				}
				prevNode = i.getCurrentNode();
			}
		}

		// Returns the first index of the given value within the list
		// CAUTION: Runtime of O(n)
		Iterator search(const T& value)
		{

			for (Iterator i = this->begin(); i != this->end(); i++)
			{
				if (i.getCurrentNode()->getData() == value)
				{
					return i;
				}
			}
			return this->end();
		}

		// Checks if the list has no linked nodes
		bool isEmpty() const
		{
			return m_Head == nullptr;
		}

		
		// Returns the address of the first node of the list
		Iterator begin() const
		{
			return Iterator(m_Head);
		}

		// 'nullptr' indicates the end of the list as it is the 'next' value of the last node
		Iterator end() const
		{
			return Iterator(nullptr);
		}

		// Allows for 'List[i]' format for accessing data by calling the '*' operator on the iterator which returns the data value
		T& operator[](const Iterator& i)
		{
			return *i;
		}

		// Defining the iterator class
		class Iterator
		{
		public:
			// Iterator defaults to the first node in the list
			Iterator()
				: m_CurrentNode(m_Head)
			{
			}

			// Iterator can be set to a specific node in the list upon instantiation
			Iterator(Node* nodeAddr)
				: m_CurrentNode(nodeAddr)
			{
			}

			// Returns the node that the iterator currently points to
			Node* getCurrentNode() const
			{
				return m_CurrentNode;
			}

			// Overwrites the '=' operator to set the current node equal to the node that is provided
			Iterator& operator=(Node* node)
			{
				this->m_CurrentNode = node;
				return *this;
			}

			// Overwrites the prefix '++' operator
			Iterator& operator++()
			{
				if (m_CurrentNode)
					m_CurrentNode = m_CurrentNode->getNext();
				return *this;
			}

			// Overwrites the postfix '++' operator
			Iterator operator++(int)
			{
				Iterator iterator = *this;
				++ *this;
				return iterator;
			}

			// Overwrites the '!=' operator
			bool operator!=(const Iterator& iterator) const
			{
				return m_CurrentNode != iterator.m_CurrentNode;
			}

			bool operator==(const Iterator& iterator) const
			{
				return m_CurrentNode == iterator.m_CurrentNode;
			}

			// Overwrites the '*' operator to return the value data
			T& operator*() const
			{
				return m_CurrentNode->getData();
			}

		private:
			Node* m_CurrentNode;
		};

	private:

		// Each represents one element of the list
		class Node
		{
		public:
			Node(const T& value) : m_Data(value), m_Next(nullptr) {}
			Node(T&& value) : m_Data(std::move(value)), m_Next(nullptr) {}

			template<typename...Args>
			Node(Args&&...args) : m_Data(T(std::forward<Args>(args)...)), m_Next(nullptr) {}

			Node* getNext() const { return m_Next; }
			void setNext(Node* addr) { m_Next = addr; }
			T& getData() { return m_Data; }
		private:
			Node* m_Next;
			T m_Data;
		};

		Node* m_Head;
		Node* m_Tail;

	};
}