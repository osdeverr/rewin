#pragma once

namespace ulib
{
	template<class T>
	struct Node
	{
		Node()
		{
			pNext = nullptr;
		}

		Node(const T& _obj)
		{
			obj = _obj;
			pNext = nullptr;
		}

		T& Get() { return obj; }
		Node* Next() { return pNext; }

		size_t Size() 
		{
			size_t size = 1;
			Node* next = pNext;
			while (next)
			{
				size++;
				next = next->pNext;
			}

			return size;
		}
		
		void Add(Node* pNode) { pNext = pNode; }

		T obj;
		Node* pNext;
	};
}