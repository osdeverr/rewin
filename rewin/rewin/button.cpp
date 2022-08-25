#include "button.h"
#include <Windows.h>

namespace rewin
{
	Button::Button(
		const Coords& pos, const Coords& size, const std::string& text)
		: Control{ pos, size, "BUTTON", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON }
	{
		mText = text;
	}
}
