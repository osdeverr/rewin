#pragma once

#include "list.h"
#include <vcruntime_new.h> // overloaded new / delete

namespace ulib
{
	template<class T>
	class RetainSlotPool
	{
	public:
		RetainSlotPool() {}
		RetainSlotPool(const RetainSlotPool&) = delete;
		~RetainSlotPool() 
		{
			for (Chunk* pChunk : mChunks)
			{
				delete pChunk;
			}
		}

		struct Chunk
		{
			bool isFree;
			T elem;
		};

		T* Get()
		{
			for (Chunk* obj : mChunks)
			{
				if (obj->isFree)
				{
					obj->isFree = false;
					return &obj->elem;
				}
			}

			Chunk* pChunk = new Chunk;
			pChunk->isFree = false;

			mChunks.Add(pChunk);
			return &pChunk->elem;
		}

		void Clear()
		{
			for (Chunk* obj : mChunks)
				obj->isFree = true;
		}

		static void Free(void* pElem)
		{
			struct Chunk
			{
				bool isFree;
				void* elem;
			};

			Chunk* pChunk = (Chunk*)((char*)pElem - offsetof(Chunk, elem));
			pChunk->isFree = true;
		}

	private:
		ulib::List<Chunk*> mChunks;
	};
}