#pragma once
#include "widget.h"
#include "coords.h"

namespace rewin
{
	class Area : public Widget
	{
	public:
		Area(const Coords& pos, const Coords& size)
			: mPos{ pos }, mSize{ size }
		{}

		template<class T>
		T* Add(const T& widget)
		{
			T* pWidget = new T(widget);
			mWidgets.Add(pWidget);
			return pWidget;
		}

		void ConnectParent(Area* pParent)
		{
			mParent = pParent;
		}

		lm::Vec2 GetAbsolutePos(const lm::Vec2& localPos)
		{
			lm::Vec2 result = localPos;

			Area* pCurr = this;

			while (pCurr)
			{
				result += pCurr->mPos.GetCoords(pCurr->mSize.GetCoords(pCurr->GetParentSize()));
				pCurr = pCurr->mParent;
			};
		}

	protected:
		Area* mParent = nullptr;
		Coords mPos, mSize;

		ulib::List<Widget*> mWidgets;

		lm::Vec2 GetParentSize()
		{
			return mParent ? mParent->mSize.GetCoords(GetParentSize()) : lm::Vec2{ 1, 1 };
		}
	};
}
