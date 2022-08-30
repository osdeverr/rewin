#pragma once
#include "control.h"
#include <Windows.h>

namespace rewin
{
	class StaticControl : public Control
	{
	public:
		StaticControl(
			const Coords& pos, const Coords& size,
			const std::string& text,
			uint32_t exStyle
		);
	};

	typedef ControlEvent<WM_COMMAND, STN_CLICKED> StaticControlClicked;
	typedef ControlEvent<WM_COMMAND, STN_DBLCLK> StaticControlDoubleClick;
	typedef ControlEvent<WM_COMMAND, STN_ENABLE> StaticControlEnable;
	typedef ControlEvent<WM_COMMAND, STN_DISABLE> StaticControlDisable;
}
