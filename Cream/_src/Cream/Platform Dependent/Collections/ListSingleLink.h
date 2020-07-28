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
			deleteList();
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

		// Inserts node with data of type 'T' after specified node
		void pushAfter(const T& value, const Iterator& index)
		{
			Node* newNode = new Node(value);
			newNode->setNext(index.getCurrentNode()->getNext());
			index.getCurrentNode()->setNext(newNode);
			
		}

		// Deletes the node at the front of the list
		void popFront()
		{
			if (m_Head != nullptr)
			{
				Node* newHead = m_Head->getNext();
				free(m_Head);
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
					free(m_Tail);
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
					free(i.getCurrentNode());
					return;
				}
				prevNode = i.getCurrentNode();
			}
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
					free(i.getCurrentNode());
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
		T& operator[](Iterator& i)
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
			T& operator*()
			{
				return m_CurrentNode->getData();
			}

		private:
			Node* m_CurrentNode;
		};

	private:
		// Frees all the memory allocated by the nodes that make up the list
		void deleteList()
		{
			Node* current = m_Head;
			Node* next;

			while (current != nullptr)
			{
				next = current->getNext();
				free(current);
				current = next;
			}
			m_Head = nullptr;
			m_Tail = nullptr;
		}

		// Each represents one element of the list
		class Node
		{
		public:
			Node(const T& value) : data(value), next(nullptr) {};
			Node* getNext() const { return next; }
			void setNext(Node* addr) { next = addr; }
			T& getData() { return data; }
		private:
			Node* next;
			T data;
		};

		Node* m_Head;
		Node* m_Tail;

	};
}