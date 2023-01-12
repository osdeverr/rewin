#pragma once
#include "slot_pool.h"
#include <tuple>

namespace ulib
{
	namespace detail
	{
		template <typename T, typename Tuple>
		struct TupleContainsType;

		template <typename T, typename... Us>
		struct TupleContainsType<T, std::tuple<Us...>> : std::disjunction<std::is_same<T, Us>...> {};
	}

	template<class... Targets>
	class SlotPoolCollection
	{
	public:
		using PoolStorage = std::tuple<ulib::SlotPool<Targets>...>;

		template<class T, class... Args>
		T* Allocate(Args&&... args)
		{
			using SlotPoolT = ulib::SlotPool<T>;

			static_assert(
				detail::TupleContainsType<SlotPoolT, PoolStorage>::value,
				"T is not supported by this SlotPoolCollection"
				);

			return std::get<SlotPoolT>(mPoolStorage).New(args...);
		}

	private:
		PoolStorage mPoolStorage;
	};
}
