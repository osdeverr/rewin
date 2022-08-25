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
		WindowCreateMode mode
	)
		: Widget{ pos, size }, mWindowClass{ windowClass }, mName{ name }
	{
		OnEvent<WindowCreate>([this](const WindowCreate& event) {
			int i = 1;

			for (auto& pChild : mChildren)
				pChild->Activate(this, i++);

			return true;
		});

		if (mode == WindowCreateMode::Now)
			Create();
	}

	void Window::Create()
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

			RegisterClassExA(&data);
		}

		auto pos = mPos.GetCoords({});
		auto size = mSize.GetCoords({});

		CreateWindowExA(0, pWndClassName, mName.c_str(), 0, (int)pos.x, (int)pos.y, (int)size.x, (int)size.y, nullptr, nullptr, hInstance, this);
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
