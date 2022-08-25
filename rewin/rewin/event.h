#pragma once
#include <cstdint>

namespace rewin
{
	enum WindowMessageType : uint32_t
	{
		WindowCreated
	};

	template<WindowMessageType T>
	struct Event
	{
		static inline constexpr auto MessageType = T;
	};
}
