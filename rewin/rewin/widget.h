#pragma once
#include <functional>
#include <unordered_map>

#include <ulib/list.h>

#include <sstream>

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

		Widget* Add(Widget* pWidget)
		{
			mChildren.Add(pWidget);

			if (mHandle)
				pWidget->InternalActivate(this, int(100u + mChildren.Size()));

			return pWidget;
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

		lm::Vec2 GetPos() const
		{
			return mPos.GetAnchoredCoords(GetParentSize(), GetSize());
		}

		lm::Vec2 GetSize() const
		{
			return mSize.GetCoords(GetParentSize());
		}

		lm::Vec2 GetParentSize() const
		{
			return mParent ? mParent->mSize.GetCoords(mParent->GetParentSize()) : lm::Vec2{ 1, 1 };
		}

		void SetPos(const Coords& pos)
		{
			mPos = pos;
			RecalculateSize();
		}

		void SetSize(const Coords& size)
		{
			mSize = size;
			RecalculateSize();
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

		uintptr_t SendWindowMessage(WindowMessageType type, WindowParam w, WindowParam l);
		void SetFont(FontHandle handle);

		const std::string& GetStringId() const { return mStringId; }
		void SetStringId(const std::string& id) { mStringId = id; }

		template<class T>
		T* FindChild(const std::string& path)
		{
			std::istringstream iss(path);
			std::vector<std::string> tokens;
			std::string token;
			while (std::getline(iss, token, '.')) {
				if (!token.empty())
					tokens.push_back(token);
			}

			return (T*)FindChild(tokens);
		}

		Widget* FindChild(const std::vector<std::string>& path, int skip = 0);

		void SetEnabled(bool enabled);

		ulib::List<Widget*>& GetChildren() { return mChildren; }
		void KillChildren();

	protected:
		WindowHandle mHandle = (WindowHandle)0;
		std::unordered_map<WindowMessageType, ulib::List<WindowMessageHandler>> mMsgHandlers;

		FontHandle mFont = (FontHandle)0;

		Widget* mParent = nullptr;
		Coords mPos, mSize;

		std::string mStringId;

		ulib::List<Widget*> mChildren;

		bool mEnabled = true;

		void InternalActivate(Widget* pParent, int id);
	};
}
