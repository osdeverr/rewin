#include <rewin/window.h>
#include <rewin/button.h>
#include <rewin/label.h>

#include <rewin/wm_events.h>

struct WindowCreated : rewin::Event<rewin::WindowMessageType::WindowCreated>
{
	int ky;
};

int main()
{
	rewin::Window window(
		rewin::WindowClass{ "rewin-example" },
		{ 100, 200 }, { 500, 350 },
		"Rewin Example",
		rewin::WindowCreateMode::Defer
	);

	window.OnEvent<rewin::WindowCreate>([&window](const rewin::WindowCreate&) {
		HFONT hFont = CreateFont(13, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET,
			OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE, TEXT("Tahoma"));

		window.ApplyForAll([hFont](rewin::Widget* pWidget) {
			SendMessage((HWND)pWidget->GetHandle(), WM_SETFONT, (WPARAM)hFont, TRUE);
		});

		return true;
	});

	window.Add(rewin::StaticControl(
		{ 0.f, 0.f, rewin::Relative },
		{ 1.f, 1.f, rewin::Relative },
		"", SS_CENTER
	));

	window.Add(rewin::Label(
		{ 0.f, 0.075f, rewin::Relative },
		{ 1.f, 0.1f, rewin::Relative },
		"Test Rewin App",
		rewin::XAlign::Center
	));

	{
		int i = 0;

		auto button = window.Add(rewin::Button(
			{ 0.5f, 0.2f, rewin::Relative },
			{ 200, 50 },
			"Click Me! (0)"
		));

		button->OnEvent<rewin::ButtonClick>([&window, &i, button](const rewin::ButtonClick&) {
			button->SetText("Click Me! (" + std::to_string(++i) + ")");
			return true;
		});
	}

	{
		int i = 0;

		auto button = window.Add(rewin::Button(
			{ 0.5f, 0.45f, rewin::Relative },
			{ 200, 50 },
			"Click Me! (0)"
		));

		button->OnEvent<rewin::ButtonClick>([&window, &i, button](const rewin::ButtonClick&) {
			button->SetText("Click Me! (" + std::to_string(++i) + ")");
			return true;
		});
	}

	{
		int i = 0;

		auto button = window.Add(rewin::Button(
			{ 0.5f, 0.7f, rewin::Relative },
			{ 200, 50 },
			"Click Me! (0)"
		));

		button->OnEvent<rewin::ButtonClick>([&window, &i, button](const rewin::ButtonClick&) {
			button->SetText("Click Me! (" + std::to_string(++i) + ")");
			return true;
		});
	}

	window.Create();
	window.Show();

	while (window.Update())
		;
}
