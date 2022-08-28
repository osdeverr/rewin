#include "window.h"
#include "wm_events.h"

#include <Windows.h>

namespace rewin
{
	namespace
	{
		LRESULT CALLBACK RewinWndProc(
			HWND hWnd,        // handle to window
			UINT uMsg,        // message identifier
			WPARAM wParam,    // first message parameter
			LPARAM lParam)    // second message parameter
		{
			if (uMsg == WM_CREATE)
			{
				Window* pWindow = (Window*)((LPCREATESTRUCT)lParam)->lpCreateParams;
				pWindow->AdoptHandle(hWnd);
				SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pWindow);
			}

			if (Window* pWindow = (Window*) GetWindowLongPtr(hWnd, GWLP_USERDATA))
				if (pWindow->InternalHandleEvent((WindowMessageType)uMsg, wParam, lParam))
					return 0;

			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	}

	Window::Window(
		const WindowClass& windowClass,
		const lm::Vec2& pos, const lm::Vec2& size,
		const std::string& name,
		WindowCreateMode mode,
		WindowFlags flags
	)
		: Widget{ pos, size }, mWindowClass{ windowClass }, mName{ name }, mFlags{ flags }
	{
		OnEvent<WindowResized>([this](const WindowResized& event) {
			mSize = event.size;

			ApplyForAll([](Widget* pWidget) {
				pWidget->RecalculateSize();
			});

			return true;
		});

		if (mode == WindowCreateMode::Now)
			Create();
	}

	void Window::Activate(Widget* pParent, int)
	{
		auto hInstance = (HINSTANCE)mWindowClass.module;
		auto pWndClassName = mWindowClass.name.c_str();

		if (!hInstance)
			hInstance = GetModuleHandleA(nullptr);

		WNDCLASSEXA data{ 0 };

		if (!GetClassInfoExA(hInstance, pWndClassName, &data))
		{
			data.cbSize = sizeof data;
			data.lpszClassName = pWndClassName;
			data.lpfnWndProc = RewinWndProc;
			data.hInstance = hInstance;
			data.style = CS_HREDRAW | CS_VREDRAW;

			RegisterClassExA(&data);
		}

		auto pos = mPos.GetCoords({});
		auto size = mSize.GetCoords({});

		DWORD style = 0;

		if (mFlags & WindowFlags_HasTitleBar)
			style |= WS_CAPTION;
		if (mFlags & WindowFlags_MaximizeButton)
			style |= WS_MAXIMIZEBOX;
		if (mFlags & WindowFlags_MinimizeButton)
			style |= WS_MINIMIZEBOX;
		if (mFlags & WindowFlags_Resizeable)
			style |= WS_SIZEBOX;
		if (mFlags & WindowFlags_InitiallyDisabled)
			style |= WS_DISABLED;
		if (mFlags & WindowFlags_InitiallyMaximized)
			style |= WS_MAXIMIZE;
		if (mFlags & WindowFlags_InitiallyMinimized)
			style |= WS_MINIMIZE;
		if (mFlags & WindowFlags_HasSysMenu)
			style |= WS_SYSMENU;

		CreateWindowExA(0, pWndClassName, mName.c_str(), style, (int)pos.x, (int)pos.y, (int)size.x, (int)size.y, pParent ? (HWND)pParent->GetHandle() : nullptr, nullptr, hInstance, this);
	}

	void Window::Show(bool show)
	{
		ShowWindow((HWND)mHandle, show);
	}

	bool Window::Update()
	{
		MSG msg = { };

		if (PeekMessageA(&msg, (HWND)mHandle, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}

		return msg.message != WM_QUIT;
	}
}
