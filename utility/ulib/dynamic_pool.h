#pragma once

#include <string.h> // memcpy
#include <vcruntime_new.h> // overloaded new / delete

#include "list.h"

namespace ulib
{
	class DynamicPool
	{
		constexpr static size_t M_STEP = 0x20000;

	public:
		inline DynamicPool() : mPages({ new uchar[M_STEP] })
		{
			mEndB = (mLastB = mBeginB = mPages.Front()) + M_STEP;
		}

		inline ~DynamicPool()
		{
			for (uchar* p : mPages) 
				delete[] p;				
		}

		inline void Clear()
		{
			for (uchar* p : mPages)
				delete[] p;
			mPages.Clear();
		}

		template<class T, class... Args>
		T* New(Args&&... args)
		{
			if ((mLastB + sizeof(T)) >= mEndB)
			{
				size_t sizeInBytes = (size_t)mEndB - (size_t)mBeginB;
				size_t allocSize = sizeof(T) + sizeInBytes + sizeInBytes;

				mEndB = (mLastB = mBeginB = new uchar[allocSize]) + allocSize;
				mPages.Add(mBeginB);
			}

			T* rv = new (mLastB) T(args...);
			mLastB += sizeof(T);

			return rv;
		}

		template<class T, class... Args>
		T* New(size_t len, Args&&... args)
		{
			size_t reqSize = sizeof(T) * len;
			if ((mLastB + reqSize) >= mEndB)
			{
				size_t sizeInBytes = (size_t)mEndB - (size_t)mBeginB;
				size_t allocSize = reqSize + sizeInBytes + sizeInBytes;

				mEndB = (mLastB = mBeginB = new uchar[allocSize]) + allocSize;
				mPages.Add(mBeginB);
			}

			T* pFirst = (T*)mLastB;
			T* pEnd = pFirst + len;

			for (T* pCurr = pFirst; pCurr != pEnd; pCurr++)
				new (pCurr) T(args...);

			mLastB += reqSize;

			return rv;
		}

		template<class T>
		static void Destroy(T* p)
		{
			p->~T();
		}

		template<class T>
		static void Destroy(size_t len, T* p)
		{
			T* pFirst = p;
			T* pEnd = pFirst + len;

			for (T* pCurr = pFirst; pCurr != pEnd; pCurr++)
				pCurr->~T();
		}

	private:

		struct
		{
			uchar* mBeginB;
			uchar* mLastB;
			uchar* mEndB;
		};

		ulib::List<uchar*> mPages;
	};
}