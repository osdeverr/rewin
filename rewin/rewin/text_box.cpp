#include "text_box.h"
#include <Windows.h>

namespace rewin
{
	namespace
	{
		int GetTextBoxWindowsStyle(TextBoxFlags flags)
		{
			auto result = 0;

			// TODO

			return result;
		}
	}

	TextBox::TextBox(
		const Coords& pos, const Coords& size,
		const std::string& text,
		TextBoxFlags flags)
		: Control{ pos, size, "EDIT", WS_CHILD | WS_VISIBLE | GetTextBoxWindowsStyle(flags) }
	{
	}

	std::string TextBox::GetText()
	{
		auto length = SendWindowMessage((WindowMessageType)WM_GETTEXTLENGTH, 0, 0);

		std::string result;
		result.resize(length + 1);

		SendWindowMessage((WindowMessageType)WM_GETTEXT, length + 1, (WindowParam)result.data());

		return result;
	}
}
