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
	{
	}

	void Control::Activate(Widget* pParent, int id)
	{
		mControlId = id;

		auto size = mSize.GetCoords(pParent->GetSize());
		auto pos = mPos.GetAnchoredCoords(pParent->GetSize(), size);

		auto root = (HWND)mRoot->GetHandle();

		mHandle = CreateWindowA(
			mClassId.c_str(),
			mText.c_str(),
			mStyle,
			(int)pos.x, (int)pos.y,
			(int)size.x, (int)size.y,
			root,
			(HMENU)mControlId,
			(HINSTANCE)GetWindowLongPtr(root, GWLP_HINSTANCE),
			nullptr
		);
	}

	void Control::RecalculateSize()
	{
		if (mHandle)
		{
			auto size = mSize.GetCoords(mParent->GetSize());
			auto pos = mPos.GetAnchoredCoords(mParent->GetSize(), size);

			MoveWindow((HWND)mHandle, (int)pos.x, (int)pos.y, (int)size.x, (int)size.y, TRUE);
		}
	}

	void Control::SetText(const std::string& text)
	{
		mText = text;
		SetWindowTextA((HWND)mHandle, mText.c_str());
	}
}
