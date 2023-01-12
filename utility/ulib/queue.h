#pragma once

#include "list.h"

namespace ulib
{
	template<class T>
	class FastQueue
	{
	public:
		FastQueue() : mIdx(0) {}

		inline void Push(const T& out) { mList.Add(out); }
		inline size_t Size() { return mList.Size(); }
		inline void Clear() { mList.Clear(); mIdx = 0; }

		inline bool Peek(T& out)
		{
			size_t size = mList.Size();
			if (!size)
				return false;

			if (mIdx == size)
			{
				Clear();
				return false;
			}

			out = mList[mIdx];
			mIdx++;

			return true;
		}	

	private:

		ulib::List<T> mList;
		int mIdx;
	};
}