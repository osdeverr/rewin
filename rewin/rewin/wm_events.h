#pragma once
#include "event.h"
#include <Windows.h>

namespace rewin
{
	class Window;

	struct WindowCreate : Event<WM_CREATE>
	{
		WindowCreate(WindowParam, WindowParam l)
			: pWindow((Window*)((LPCREATESTRUCT)l)->lpCreateParams)
		{}

		Window* pWindow;
	};
}
