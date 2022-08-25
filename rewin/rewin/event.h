#pragma once
#include <cstdint>

namespace rewin
{
	enum WindowMessageType : uint32_t
	{
		WindowCreated
	};

	typedef uintptr_t WindowParam;

	template<auto T>
	struct Event
	{
		static inline constexpr auto MessageType = (WindowMessageType) T;
	};
}
