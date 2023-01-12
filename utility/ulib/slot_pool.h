#pragma once

#include <malloc.h>
#include <string.h> // memcpy
#include <vcruntime_new.h> // overloaded new / delete

#include "list.h"

namespace ulib
{
	template<class T>
	class SlotPool
	{
		constexpr static size_t C_STEP = 0x400;
		constexpr static size_t M_STEP = sizeof(T) * C_STEP;

	public:

		inline SlotPool()
		{
			mEndB = (mLastB = mBeginB = (uchar*)::malloc(M_STEP)) + M_STEP;
#ifdef _DEBUG
			assert(mBeginB && "Out of memory in SlotPool<T>()");
#endif
		}

		inline ~SlotPool()
		{
			DestroyFullPages();

			for (T* ptr = mBegin; ptr != mLast; ptr++)
				ptr->~T();
			delete[] mBeginB;		
		}

		template<class... Args>
		inline T* New(Args&&... args)
		{
			if (mLast == mEnd)
			{
				mFullPages.Add(mBegin);
				mEndB = (mLastB = mBeginB = new uchar[M_STEP]) + M_STEP;
#ifdef _DEBUG
				assert(mBeginB && "Out of memory in SlotPool<T>()");
#endif
			}

			T* rv = new (mLast) T(args...);
			mLast++;

			return rv;
		}

		inline void Clear()
		{
			DestroyFullPages();
			mFullPages.Clear();

			for (T* ptr = mBegin; ptr != mLast; ptr++)
				ptr->~T();
		}

	private:

		inline void DestroyFullPages()
		{
			for (T* pPage : mFullPages)
			{
				for (T* ptr = pPage; ptr != pPage + C_STEP; ptr++)
					ptr->~T();

				delete[] (uchar*)pPage;
			}
		}

		union
		{
			struct
			{
				T* mBegin;
				T* mLast;
				T* mEnd;
			};

			struct
			{
				uchar* mBeginB;
				uchar* mLastB;
				uchar* mEndB;
			};
		};

		ulib::List<T*> mFullPages; // *plaks
	};
}