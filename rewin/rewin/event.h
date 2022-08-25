#pragma once
#include <cstdint>

namespace rewin
{
	enum WindowMessageType : uint32_t
	{
		WindowCreated
	};

	typedef uintptr_t WindowParam;

	class Widget;

	template<auto T>
	struct Event
	{
		static inline constexpr auto MessageType = (WindowMessageType) T;

		static bool ShouldProcess(Widget*, WindowParam, WindowParam) { return true; }
	};
}
