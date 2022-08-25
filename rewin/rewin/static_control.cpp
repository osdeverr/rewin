#include "static_control.h"
#include <Windows.h>

namespace rewin
{
	StaticControl::StaticControl(
		const Coords& pos, const Coords& size,
		const std::string& text,
		uint32_t exStyle)
		: Control{ pos, size, "STATIC", WS_CHILD | WS_VISIBLE | exStyle }
	{
		mText = text;
	}
}
