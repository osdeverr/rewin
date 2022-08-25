#pragma once
#include "control.h"

#include <Windows.h>

namespace rewin
{
	class Button : public Control
	{
	public:
		Button(
			const Coords& pos, const Coords& size,
			const std::string& text
		);
	};

	typedef ControlEvent<WM_COMMAND, BN_CLICKED> ButtonClick;
}
