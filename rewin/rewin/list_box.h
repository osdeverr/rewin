#pragma once
#include "control.h"
#include <Windows.h>

namespace rewin
{
	enum ListBoxFlags
	{
		ListBoxFlags_None = 0,
		ListBoxFlags_Strings = 1 << 0,
		ListBoxFlags_ViewOnly = 1 << 1,
		ListBoxFlags_UseTabStops = 1 << 2
	};

	class ListBox : public Control
	{
	public:
		ListBox(
			const Coords& pos, const Coords& size,
			ListBoxFlags flags
		);

		int AddItem(const std::string& value);
	};
}
