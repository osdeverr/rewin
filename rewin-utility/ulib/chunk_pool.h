#pragma once

#include "list.h"

namespace ulib
{
	template<class T>
	class ChunkPool
	{
	public:

		struct Chunk
		{
			bool isFree;
			T obj;
		};

		ChunkPool() {}
		~ChunkPool()
		{
			for (Chunk* ptr : mArray)
				delete ptr;
		}

		static void free(T* pV)
		{
			ChunkPool* pChunk = (Chunk*)((char*)pV - offsetof(chunk, pac));
			pChunk->isFree = true;
		}

		spp::Packet* create()
		{
			for (chunk* pChunk : mArray)
				if (pChunk->isFree)
				{
					pChunk->isFree = false;
					return &pChunk->pac;
				}

			chunk* pChunk = new chunk;
			pChunk->isFree = false;

			mArray.Add(pChunk);
			return &pChunk->pac;
		}

		spp::Packet* create(unsigned int t)
		{
			spp::Packet* pPack = create();
			pPack->Reset(t);
		}

		void free_all()
		{
			for (chunk* ptr : mArray)
				ptr->isFree = true;
		}

		size_t allocated_size()
		{
			size_t cnt = 0;
			for (chunk* ptr : mArray)
				if (!ptr->isFree)
					cnt++;
			return cnt;
		}


	private:
		ulib::List<Chunk*> mArray;
	};
}