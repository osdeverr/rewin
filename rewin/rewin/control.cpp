#include "control.h"

#include <Windows.h>

namespace rewin
{
	Control::Control(
		const Coords& pos, const Coords& size,
		const std::string& classId,
		uint32_t style
	)
		: Widget{ pos, size }, mClassId{ classId }, mStyle{ style }
	{}

	void Control::Activate(Widget* pParent, int id)
	{
		mControlId = id;

		auto size = mSize.GetCoords(pParent->GetSize());
		auto pos = mPos.GetCoords(pParent->GetSize());

		auto parent = (HWND)pParent->GetHandle();

		mHandle = CreateWindowA(
			mClassId.c_str(),
			mText.c_str(),
			mStyle,
			(int)pos.x, (int)pos.y,
			(int)size.x, (int)size.y,
			parent,
			(HMENU)mControlId,
			(HINSTANCE)GetWindowLongPtr(parent, GWLP_HINSTANCE),
			nullptr
		);
	}

	void Control::SetText(const std::string& text)
	{
		mText = text;
		SetWindowTextA((HWND)mHandle, mText.c_str());
	}
}
