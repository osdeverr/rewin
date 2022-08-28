#pragma once
#include "widget.h"

#include <string>
#include <Windows.h>

namespace rewin
{
	struct WindowClass
	{
		std::string name;
		void* module;

		WindowClass(const std::string& name, void* module = nullptr)
			: name{ name }, module{ module }
		{}
	};

	enum class WindowCreateMode
	{
		Now,
		Defer
	};

	enum WindowFlags
	{
		WindowFlags_None = 0,
		WindowFlags_HasTitleBar = 1 << 0,
		WindowFlags_MinimizeButton = 1 << 1,
		WindowFlags_MaximizeButton = 1 << 2,
		WindowFlags_Resizeable = 1 << 3,
		WindowFlags_InitiallyDisabled = 1 << 4,
		WindowFlags_InitiallyMinimized = 1 << 5,
		WindowFlags_InitiallyMaximized = 1 << 6,
		WindowFlags_HasSysMenu = 1 << 7,

		WindowFlags_DefaultWindowStyle = WindowFlags_HasTitleBar | WindowFlags_MinimizeButton | WindowFlags_MaximizeButton | WindowFlags_Resizeable | WindowFlags_HasSysMenu,
	};

	struct WindowResized : Event<WM_SIZE>
	{
		lm::Vec2 size;

		WindowResized(WindowParam w, WindowParam l) { size = { float(l & 0xFFFF), float((l & 0xFFFF0000) >> 16) }; }
	};

	class Window : public Widget
	{
	public:
		Window(
			const WindowClass& windowClass,
			const lm::Vec2& pos, const lm::Vec2& size,
			const std::string& name,
			WindowCreateMode mode = WindowCreateMode::Now,
			WindowFlags flags = WindowFlags_DefaultWindowStyle
		);

		virtual void Activate(Widget*, int) override;

		void Show(bool show = true);

		void Create()
		{
			InternalActivate(nullptr, -1);
		}

		bool Update();

		const std::string& GetName() const
		{
			return mName;
		}

	private:
		WindowClass mWindowClass;
		WindowFlags mFlags;
		std::string mName;
	};
}
