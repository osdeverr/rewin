#pragma once

#include "list.h"

namespace ulib
{
	template<class IntegerT>
	class UniqueGenerator
	{
	public:
		bool IsExist(IntegerT value)
		{
			for (IntegerT it : mValues)
			{
				if (it == value)
					return true;
			}

			return false;
		}

		IntegerT Generate()
		{
			while (true)
			{
				IntegerT value = Rand();
				if (value == 0)
					continue;

				if (IsExist(value))
					continue;

				mValues.Add(value);
				return value;
			}
		}

		void Remove(IntegerT value)
		{
			mValues.FastRemove(value);
		}

		size_t Size() { return mValues.Size(); }

	private:
		IntegerT Rand()
		{
			IntegerT value = 0;
			size_t base = sizeof(IntegerT) * 8;
			while (base)
			{
				base -= sizeof(short) * 8;
				value += IntegerT(rand() << base);
			}

			return value;
		}

		ulib::List<IntegerT> mValues;
	};
}