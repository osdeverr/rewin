#pragma once

#include <string.h> // memcpy
#include <vcruntime_new.h> // overloaded new / delete
#include <xutility> // std::iterator

#ifdef _DEBUG
#include <assert.h>
#endif

#include "types.h"

namespace ulib
{
	template<class T>
	class List
	{
		constexpr static size_t C_STEP = 16;
		constexpr static size_t M_STEP = sizeof(T) * C_STEP;
	public:

		using value_type = T;
		using pointer = value_type*;
		using reference = value_type&;

		template<class T>
		struct Iterator
		{
			using iterator_category = std::random_access_iterator_tag;
			using value_type = T;
			using difference_type = ptrdiff_t;
			using pointer = value_type*;
			using reference = value_type&;

			Iterator() {}
			Iterator(T* _ptr) : ptr(_ptr) {}
			Iterator(const Iterator<T>& it) : ptr(it.ptr) {}

			bool operator!=(const Iterator<T>& it) const { return ptr != it.ptr; }
			bool operator==(const Iterator<T>& it) const { return ptr == it.ptr; }

			T& operator*() const { return *ptr; }
			Iterator<T>& operator++() { ++ptr; return *this; }       // Prefix increment operator.
			Iterator<T>& operator++(int) { ptr++; return *this; }     // Postfix increment operator.
			Iterator<T>& operator--() { --ptr; return *this; }
			Iterator<T>& operator--(int) { ptr--; return *this; }

			Iterator<T> operator+(int ky) const { return ptr + ky; }
			Iterator<T> operator-(int ky) const { return ptr - ky; }

			// Iterator<T> operator+(Iterator<T> ky) const { return (T*)(ptrdiff_t(ptr) + ptrdiff_t(ky.ptr)); }
			ptrdiff_t operator-(Iterator<T> ky) const { return ptr - ky.ptr; }

			bool operator<(Iterator<T> ky) const { ptr < ky.ptr; }
			bool operator>(Iterator<T> ky) const { ptr > ky.ptr; }

			T* operator->() const { return ptr; }

			T* ptr;
		};

		typedef Iterator<T> iterator;
		typedef Iterator<const T> const_iterator;

		inline List()
		{
			mEndB = (mLastB = mBeginB = (uchar*)new char[M_STEP]) + M_STEP;
#ifdef _DEBUG
			assert(mBegin && "Out of memory in List<T>::List()");
#endif
		}

		inline List(size_t capacity)
		{
			size_t allocSize = sizeof(T) * capacity;
			mEndB = (mLastB = mBeginB = (uchar*)new char[allocSize]) + allocSize;
#ifdef _DEBUG
			assert(mBegin && "Out of memory in List<T>::List(size_t)");
#endif
		}

		inline List(std::initializer_list<T> init)
		{
			size_t allocSize = init.size() * sizeof(T);
			mEndB = mLastB = (mBeginB = (uchar*)new char[allocSize]) + allocSize;
#ifdef _DEBUG
			assert(mBegin && "Out of memory in List<T>::List(initializer_list<T>)");
#endif
			memcpy(mBegin, init.begin(), allocSize);
		}

		inline List(const List& obj)
		{
			size_t allocSize = obj.CapacityInBytes();
			size_t dataSize = obj.SizeInBytes();

			mEndB = (mLastB = mBeginB = (uchar*)new char[allocSize]) + allocSize;
#ifdef _DEBUG
			assert(mBegin && "Out of memory in List<T>::List(const List&)");
#endif
			/*
			::memcpy(mBegin, obj.mBegin, dataSize);
			mLastB += dataSize;
			*/

			mLastB += dataSize;
			T* ptr = mBegin;
			const T* ptr2 = obj.mBegin;

			for (; ptr != mLast; ptr++, ptr2++)
				new(ptr) T(*ptr2);

		}

		inline ~List()
		{
			for (T* ptr = mBegin; ptr != mLast; ptr++)
				ptr->~T();
			::delete[] (mBeginB);
		}

		inline List& operator=(const List& obj)
		{
			size_t allocSize = obj.CapacityInBytes();
			size_t dataSize = obj.SizeInBytes();

			for (T* ptr = mBegin; ptr != mLast; ptr++)
				ptr->~T();

			if (SizeInBytes() < dataSize)
			{
				delete[] mBeginB;
				mEndB = (mLastB = mBeginB = new uchar[allocSize]) + allocSize;
			}
			else
			{
				mLast = mBegin;
			}

			mLastB += dataSize;
			T* ptr = mBegin;
			const T* ptr2 = obj.mBegin;

			for (; ptr != mLast; ptr++, ptr2++)
				new(ptr) T(*ptr2);

			return *this;
		}

		inline size_t Size() const { return mLast - mBegin; }
		inline size_t Capacity() const { return mEnd - mBegin; }
		inline size_t SizeInBytes() const { return mLastB - mBeginB; }
		inline size_t CapacityInBytes() const { return mEndB - mBeginB; }
		inline bool Empty() const { return mLast == mBegin; }

		inline iterator Begin() { return mBegin; }
		inline iterator End() { return mLast; }
		inline const_iterator Begin() const { return mBegin; }
		inline const_iterator End() const { return mLast; }
		inline iterator ReverseBegin() { return mLast - 1; }
		inline iterator ReverseEnd() { return mBegin - 1; }
		inline const_iterator ReverseBegin() const { return mLast - 1; }
		inline const_iterator ReverseEnd() const { return mBegin - 1; }

		inline void InsertBack(const T* pElems, size_t count)
		{
			size_t sizeInBytes = SizeInBytes();
			size_t reqSize = sizeInBytes + count * sizeof(T);
			if (Capacity() < reqSize)
			{
				ReallocateMemory(sizeInBytes, reqSize);
			}

			auto newLast = mLast + count;
			for (; mLast != newLast; mLast++, pElems++)
				new(mLast) T(std::move(*pElems));
		}

		inline void Remove(const T& elem)
		{
			for (auto it = mBegin; it != mLast; it++)
			{
				if (*it == elem)
				{
					Erase(it);
					return;
				}
			}
		}

		inline void FastRemove(const T& elem)
		{
			for (auto it = mBegin; it != mLast; it++)
			{
				if (*it == elem)
				{
					FastErase(it);
					return;
				}
			}
		}

		inline size_t size() { return mLast - mBegin; }

		inline T& Front() 
		{ 
#ifdef _DEBUG
			assert(Size() && "Attempt get out of range element in List<T>::Front");
#endif
			return *Begin(); 
		}
		inline const T& Front() const 
		{ 
#ifdef _DEBUG
			assert(Size() && "Attempt get out of range element in List<T>::Front");
#endif
			return *Begin(); 
		}
		inline T& Back() 
		{ 
#ifdef _DEBUG
			assert(Size() && "Attempt get out of range element in List<T>::Back");
#endif
			return *ReverseBegin(); 
		}
		inline const T& Back() const 
		{ 
#ifdef _DEBUG
			assert(Size() && "Attempt get out of range element in List<T>::Back");
#endif
			return *ReverseBegin(); 
		}

		// for foreach
		inline iterator begin() { return mBegin; }
		inline iterator end() { return mLast; }
		inline const_iterator begin() const { return mBegin; }
		inline const_iterator end() const { return mLast; }

		inline void Add(const T& o)
		{
			if (mLast == mEnd)
			{
				size_t sizeInBytes = SizeInBytes();
				ReallocateMemory(sizeInBytes, sizeInBytes);
			}

			new(mLast) T(std::move(o));
			mLast++;
		}

		inline T& At(size_t i) 
		{ 
#ifdef _DEBUG
			assert(Size() > i && "Attempt get out of range element in List<T>::At");
#endif
			return mBegin[i]; 
		}
		inline const T& At(size_t i) const 
		{
#ifdef _DEBUG
			assert(Size() > i && "Attempt get out of range element in List<T>::At");
#endif
			return mBegin[i]; 
		}

		inline void Erase(iterator it)
		{
#ifdef _DEBUG
			assert(it.ptr >= mBegin && "Attempt erase out of range element in List<T>::Erase");
			assert(it.ptr < mLast && "Attempt erase out of range element in List<T>::Erase");
#endif
			it.ptr->~T();
			T* from = it.ptr + 1;
			::memmove(it.ptr, from, mLastB - (uchar*)from);
			mLast--;
		}

		inline void Erase(size_t i) { Erase(mBegin + i); }
		inline int Index(const_iterator it) const { return int(it.ptr - mBegin); }
		inline int Index(iterator it) const { return int(it.ptr - mBegin); }

		inline void FastErase(iterator it)
		{
#ifdef _DEBUG
			assert(it.ptr >= mBegin && "Attempt erase out of range element in List<T>::FastErase");
			assert(it.ptr < mLast && "Attempt erase out of range element in List<T>::FastErase");
#endif

			T* pBack = mLast - 1;
			it.ptr->~T();

			if (it.ptr != pBack)
				memcpy(it.ptr, pBack, sizeof(T));

			mLast--;
		}

		inline void FastErase(size_t i) { FastErase(mBegin + i); }

		inline void Clear() 
		{ 
			for (T* ptr = mBegin; ptr != mLast; ptr++)
				ptr->~T();
			mLast = mBegin; 
		}

		inline T& operator[](int i) { return At(i); }
		inline const T& operator[](int i) const { return At(i); }

		inline void Pop()
		{
#ifdef _DEBUG
			assert(Size() && "Attempt pop element in empty list in List<T>::Pop");
#endif
			mLast--;
			mLast->~T();
		}

		inline bool Reserve(size_t s) 
		{
			if (Capacity() >= s)
				return true;

			uchar* newMem = (uchar*)new uchar[s];
			if (!newMem)
				return false;

			T* oldBegin = mBegin;
			size_t oldSizeInBytes = SizeInBytes();

			mEndB = (mLastB = mBeginB = newMem) + s;
			mLastB += oldSizeInBytes;

			::memcpy(mBegin, oldBegin, oldSizeInBytes);
			delete[] (uchar*)oldBegin;

			return true;
		}

		inline T* Data()
		{
			return mBegin;
		}

	private:

		inline void ReallocateMemory(size_t currentSizeInBytes, size_t additionalSizeInBytes)
		{
			size_t oldSizeInBytes = currentSizeInBytes;
			size_t allocSizeInBytes = oldSizeInBytes + additionalSizeInBytes;
			T* oldBegin = mBegin;

			mEndB = (mLastB = mBeginB = new uchar[allocSizeInBytes]) + allocSizeInBytes;
#ifdef _DEBUG
			assert(mBegin && "Out of memory in List<T>::Add");
#endif
			mLastB += oldSizeInBytes;

			::memcpy(mBegin, oldBegin, oldSizeInBytes);
			delete[]((uchar*)oldBegin);
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
	};

	// use for Cont<T*> where T* allocated as new T
	template<class ContT>
	void LoopDelete(const ContT& cont) { for (auto ptr : cont) delete ptr; }

	// use for Cont<T*> where T* allocated as new (buf) T
	template<class ContT>
	void LoopDestroy(const ContT& cont) { for (auto ptr : cont) ptr->~T(); }
}