#pragma once
#include <functional>
#include <unordered_map>

#include <ulib/list.h>

#include "event.h"

namespace rewin
{
	typedef void* WindowHandle;

	typedef std::function<void(WindowParam w, WindowParam l)> WindowMessageHandler;

	class Widget
	{
	public:
		Widget() = default;
		Widget(const Widget&) = delete;
		Widget(Widget&&) = default;

		template<class T, class H>
		void OnEvent(const H& handler)
		{
			mMsgHandlers[T::MessageType].Add([this, handler](WindowParam w, WindowParam l)
			{
				handler(T{w, l});
			});
		}

		bool InternalHandleEvent(WindowMessageType type, WindowParam w, WindowParam l)
		{
			bool handled = false;

			for (auto& handler : mMsgHandlers[type])
			{
				handler(w, l);
				handled = true;
			}

			return handled;
		}

		WindowHandle GetHandle() const
		{
			return mHandle;
		}

		void AdoptHandle(WindowHandle handle)
		{
			mHandle = handle;
		}

	protected:
		WindowHandle mHandle = (WindowHandle) -1;
		std::unordered_map<WindowMessageType, ulib::List<WindowMessageHandler>> mMsgHandlers;

		virtual void Activate() {}
	};
}
