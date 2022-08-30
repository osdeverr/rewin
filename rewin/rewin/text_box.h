#pragma once
#include "control.h"

#include <Windows.h>

namespace rewin
{
	enum TextBoxFlags
	{

	};

	class TextBox : public Control
	{
	public:
		TextBox(
			const Coords& pos, const Coords& size,
			const std::string& text,
			TextBoxFlags flags
		);

		std::string GetText();
	};

	typedef ControlEvent<WM_COMMAND, EN_CHANGE> TextBoxContentChanged;
}
