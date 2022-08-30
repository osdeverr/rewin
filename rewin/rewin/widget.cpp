#include "widget.h"

#include <Windows.h>

namespace rewin
{
	bool Widget::InternalHandleEvent(WindowMessageType type, WindowParam w, WindowParam l)
	{
		bool handled = false;

		for (auto& handler : mMsgHandlers[type])
			handled |= handler(w, l);

		for (auto& pChild : mChildren)
			handled |= pChild->InternalHandleEvent(type, w, l);

		return handled;
	}

	lm::Vec2 Widget::GetAbsolutePos(const lm::Vec2& localPos) const
	{
		lm::Vec2 result = localPos;

		const Widget* pCurr = this;

		while (pCurr)
		{
			result += pCurr->mPos.GetCoords(pCurr->mSize.GetCoords(pCurr->GetParentSize()));
			pCurr = pCurr->mParent;
		};

		return result;
	}

	uintptr_t Widget::SendWindowMessage(WindowMessageType type, WindowParam w, WindowParam l)
	{
		return SendMessage((HWND) mHandle, (UINT) type, w, l);
	}

	void Widget::SetFont(FontHandle handle)
	{
		if (mFont == handle)
			return;

		auto oldFont = mFont;

		mFont = handle;

		ApplyForAll([handle, oldFont](rewin::Widget* pWidget) {
			if (pWidget->mFont == oldFont)
			{
				pWidget->mFont = handle;

				if (pWidget->GetHandle())
					pWidget->SendWindowMessage((WindowMessageType)WM_SETFONT, (WindowParam)pWidget->mFont, true);
			}
		});
	}

	void Widget::InternalActivate(Widget* pParent, int id)
	{
		mParent = pParent;

		if (mParent && mParent->mRoot)
			mRoot = mParent->mRoot;

		if (mParent && !mFont)
			mFont = mParent->mFont;

		Activate(mParent, id);
		SetEnabled(mEnabled);

		int i = 1;

		for (auto& pChild : mChildren)
			pChild->InternalActivate(this, 100 + i++);

		if (mFont)
		{
			ApplyForAll([this](rewin::Widget* pWidget) {
				if (pWidget->GetHandle())
					pWidget->SendWindowMessage((WindowMessageType)WM_SETFONT, (WindowParam)pWidget->mFont, true);
			});
		}
	}

	Widget* Widget::FindChild(const std::vector<std::string>& path, int skip)
	{
		if (skip >= path.size())
			return nullptr;

		for (auto& pChild : mChildren)
			if (pChild->mStringId == path[skip])
				if (skip == path.size() - 1)
					return pChild;
				else
					return pChild->FindChild(path, skip + 1);

		return nullptr;
	}

	void Widget::SetEnabled(bool enabled)
	{
		if (mHandle)
			EnableWindow((HWND)mHandle, enabled);

		mEnabled = enabled;
	}

	void Widget::KillChildren()
	{
		for (auto& pChild : mChildren)
			delete pChild;

		mChildren.Clear();
	}
}
