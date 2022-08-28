#pragma once
#include <functional>
#include <unordered_map>

#include <ulib/list.h>

#include "event.h"
#include "coords.h"

#include "font.h"

namespace rewin
{
	typedef void* WindowHandle;

	typedef std::function<bool(WindowParam w, WindowParam l)> WindowMessageHandler;

	class Widget
	{
	public:
		Widget(const Coords& pos, const Coords& size)
			: mPos{ pos }, mSize{ size }
		{}

		Widget(const Widget&) = default;
		Widget(Widget&&) = default;

		template<class T, class H>
		void OnEvent(const H& handler)
		{
			mMsgHandlers[T::MessageType].Add([this, handler](WindowParam w, WindowParam l)
			{
				bool wants = T::ShouldProcess(this, w, l);

				if (wants)
					return handler(T{ w, l });
				
				return wants;
			});
		}

		template<class T, class H>
		void OnRawEvent(T type, const H& handler)
		{
			mMsgHandlers[(WindowMessageType) type].Add([this, handler](WindowParam w, WindowParam l)
			{
				return handler(w, l);
			});
		}

		bool InternalHandleEvent(WindowMessageType type, WindowParam w, WindowParam l);

		WindowHandle GetHandle() const
		{
			return mHandle;
		}

		void AdoptHandle(WindowHandle handle)
		{
			mHandle = handle;
		}

		template<class T>
		T* Add(const T& widget)
		{
			T* pWidget = new T(widget);
			mChildren.Add(pWidget);

			if(mHandle)
				pWidget->InternalActivate(this, int(100u + mChildren.Size()));

			return pWidget;
		}

		lm::Vec2 GetAbsolutePos(const lm::Vec2& localPos) const;

		lm::Vec2 GetSize() const
		{
			return mSize.GetCoords(GetParentSize());
		}

		lm::Vec2 GetParentSize() const
		{
			return mParent ? mParent->mSize.GetCoords(mParent->GetParentSize()) : lm::Vec2{ 1, 1 };
		}

		template<class F>
		void ApplyForAll(const F& f)
		{
			f(this);

			for (auto& pChild : mChildren)
				pChild->ApplyForAll(f);
		}

		virtual void Activate(Widget* pParent, int id) {}
		virtual void RecalculateSize() {}

		void SendWindowMessage(WindowMessageType type, WindowParam w, WindowParam l);
		void SetFont(FontHandle handle);

	protected:
		WindowHandle mHandle = (WindowHandle)0;
		std::unordered_map<WindowMessageType, ulib::List<WindowMessageHandler>> mMsgHandlers;

		FontHandle mFont = (FontHandle)0;

		Widget* mParent = nullptr;
		Coords mPos, mSize;

		ulib::List<Widget*> mChildren;

		void InternalActivate(Widget* pParent, int id);
	};
}
