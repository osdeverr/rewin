#include <rewin/window.h>
#include <rewin/button.h>
#include <rewin/label.h>
#include <rewin/image.h>
#include <rewin/text_box.h>

#include <rewin/blueprint.h>

#include <rewin/wm_events.h>

#include <WinUser.h> 

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

	HFONT fontTahomaNormal = CreateFont(13, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, TEXT("Tahoma"));

	HFONT fontTahomaBig = CreateFont(13, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		FW_BOLD, TEXT("Tahoma"));

	window.SetFont(fontTahomaNormal);

	rewin::Blueprint testWindowContents{
		{
			"MainArea",
			rewin::StaticControl(
				{ 0.f, 0.f, rewin::Relative },
				{ 1.f, 1.f, rewin::Relative },
				"", SS_CENTER
			),
			{
				rewin::Label(
					{ 0.f, 0.075f, rewin::Relative },
					{ 1.f, 0.1f, rewin::Relative },
					"Test Rewin App",
					rewin::XAlign::Center
				)
			}
		}
	};

	testWindowContents.LoadTo(&window);

	/*
	window.Add(rewin::StaticControl(
		{ 0.f, 0.f, rewin::Relative },
		{ 1.f, 1.f, rewin::Relative },
		"", SS_CENTER
	));

	window.Add(rewin::Image(
		{ 0.1f, 0.2f, rewin::Relative },
		{ 0.5f, 0.5f, rewin::Relative },
		rewin::ImageResource::LoadFromFile("D:/ky.bmp")
	))->OnEvent<rewin::StaticControlDoubleClick>([](const rewin::StaticControlDoubleClick&) {
		printf("KY\n");
		return true;
	});

	window.Add(rewin::Label(
		{ 0.f, 0.075f, rewin::Relative },
		{ 1.f, 0.1f, rewin::Relative },
		"Test Rewin App",
		rewin::XAlign::Center
	))->SetFont(fontTahomaBig);

	auto pTextBox = window.Add(rewin::TextBox(
		{ 0.3f, 0.1f, rewin::Relative },
		{ 400, 30 },
		"",
		{}
	));

	window.Add(rewin::Button(
		pTextBox->GetPos() + lm::Vec2{ 0, 35 },
		{ 50, 20 },
		"Echo"
	))->OnEvent<rewin::ButtonClick>([pTextBox, &window](const rewin::ButtonClick&) {
		MessageBoxA((HWND) window.GetHandle(), pTextBox->GetText().c_str(), "Echo", 0);
		return true;
	});

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
	*/

	window.Create();
	window.Show();

	while (window.Update())
		;
}
