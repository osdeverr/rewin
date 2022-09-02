#include "list_box.h"

#include <CommCtrl.h>

namespace rewin
{
	static uint32_t ListBoxFlagsToStyle(ListBoxFlags flags)
	{
		uint32_t result = 0;

		if (flags & ListBoxFlags_Strings)
			result |= LBS_HASSTRINGS;
		if (flags & ListBoxFlags_ViewOnly)
			result |= LBS_NOSEL;
		if (flags & ListBoxFlags_UseTabStops)
			result |= LBS_USETABSTOPS;

		return result;
	}

	ListBox::ListBox(const Coords& pos, const Coords& size, ListBoxFlags flags)
		: Control { pos, size, WC_LISTBOX, WS_TABSTOP | WS_VISIBLE | WS_CHILD | ListBoxFlagsToStyle(flags) }
	{
	}

	int ListBox::AddItem(const std::string& value)
	{
		return SendWindowMessage((WindowMessageType)LB_ADDSTRING, 0, (WindowParam)value.c_str());
	}
}
