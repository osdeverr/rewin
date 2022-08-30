#include <rewin/window.h>
#include <rewin/button.h>
#include <rewin/label.h>
#include <rewin/image.h>
#include <rewin/text_box.h>

#include <rewin/blueprint.h>

#include <rewin/wm_events.h>

#include <CommCtrl.h>
#include <WinUser.h>

#pragma comment(lib, "comctl32.lib")

struct WindowCreated : rewin::Event<rewin::WindowMessageType::WindowCreated>
{
	int ky;
};

void PrintKy(rewin::Widget& widget, int tab = 0)
{
	for (auto i = 0; i < tab; i++)
		printf("\t");

	printf("- '%s' @ %p\n", widget.GetStringId().c_str(), &widget);

	for (auto& child : widget.GetChildren())
		PrintKy(*child, tab + 1);
}

int main()
{
	INITCOMMONCONTROLSEX commctrl;
	commctrl.dwSize = sizeof commctrl;
	commctrl.dwICC = 0xFFFFFFFF;

	InitCommonControlsEx(&commctrl);

	rewin::Window window(
		rewin::WindowClass{ "rewin-example" },
		{ 100, 200 }, { 500, 350 },
		"Rewin Example",
		rewin::WindowCreateMode::Defer
	);

	HFONT fontTahomaNormal = CreateFont(13, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, TEXT("Tahoma"));

	HFONT fontTahomaBold = CreateFont(13, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		FW_BOLD, TEXT("Tahoma"));

	HFONT fontTahomaBig = CreateFont(36, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		FW_BOLD, TEXT("PF DinDisplay Pro"));

	HFONT fontTahomaItalics = CreateFont(13, 0, 0, 0, FW_DONTCARE, TRUE, FALSE, FALSE, ANSI_CHARSET,
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, TEXT("Tahoma"));

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
				{
					"WelcomeLabel",
					rewin::Label(
						{ 0.5f, 0.2f, rewin::Relative, rewin::CoordAnchor::Center },
						{ 1.f, 0.1f, rewin::Relative },
						"PlakMP Launcher",
						rewin::XAlign::Center
					)
				},
				{
					"NameInputHint",
					rewin::Label(
						{0.5f, 0.42f, rewin::Relative, rewin::CoordAnchor::Center},
						{250, 15},
						"Enter your PlakMP username:",
						rewin::XAlign::Left
					)
				},
				{
					"NameInput",
					rewin::TextBox(
						{0.5f, 0.5f, rewin::Relative, rewin::CoordAnchor::Center},
						{250, 20},
						"",
						{}
					)
				},
				{
					"NameInputRules",
					rewin::Label(
						{0.5f, 0.58f, rewin::Relative, rewin::CoordAnchor::Center},
						{250, 15},
						"(3-32 characters!)",
						rewin::XAlign::Right
					)
				},
				{
					"RunButton",
					rewin::Button(
						{ 0.5f, 0.8f, rewin::Relative, rewin::CoordAnchor::Center },
						{100, 40},
						"Run PlakMP"
					)
				}
			}
		}
	};

	testWindowContents.FindWidget<rewin::Label>("MainArea.WelcomeLabel")->SetFont(fontTahomaBig);
	testWindowContents.FindWidget<rewin::Label>("MainArea.NameInputHint")->SetFont(fontTahomaBold);

	// testWindowContents.FindWidget<rewin::Label>("MainArea.NameInputRules")->SetFont(fontTahomaItalics);
	testWindowContents.FindWidget<rewin::Label>("MainArea.NameInputRules")->SetEnabled(false);

	testWindowContents.LoadTo(&window);

	int tab = 0;

	PrintKy(window, tab);

	auto pNameInput = window.FindChild<rewin::TextBox>("MainArea.NameInput");
	auto pRunButton = window.FindChild<rewin::Button>("MainArea.RunButton");

	pNameInput->OnEvent<rewin::TextBoxContentChanged>([pNameInput, pRunButton](const rewin::TextBoxContentChanged&) {
		auto size = pNameInput->GetText().size();
		pRunButton->SetEnabled(size > 3 && size < 32);

		return true;
	});

	pRunButton->OnEvent<rewin::ButtonClick>([pNameInput, pRunButton](const rewin::ButtonClick&) {
		pNameInput->SetText("");

		return true;
	});

	pRunButton->SetEnabled(false);
	window.FindChild<rewin::Label>("MainArea.NameInputRules")->SetEnabled(false);

	/*
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
	))->SetFont(fontTahomaBig);

	auto pTextBox = window.Add(rewin::TextBox(
		{ 0.1f, 0.1f, rewin::Relative },
		{ 100, 25 },
		"",
		{}
	));

	window.Add(rewin::Button(
		pTextBox->GetPos() + lm::Vec2{ 0, 50 },
		{ 50, 20 },
		"Echo"
	))->OnEvent<rewin::ButtonClick>([pTextBox, &window](const rewin::ButtonClick&) {
		MessageBoxA((HWND) window.GetHandle(), pTextBox->GetText().c_str(), "Echo", 0);
		return true;
	});

	auto button = window.Add(
		rewin::Button(
			{ 0.5f, 0.5f, rewin::Relative, rewin::CoordAnchor::TopLeft },
			{ 100, 100 },
			"OK"
		));

	int ky = 0;

	button->OnEvent<rewin::ButtonClick>([&ky, button](const rewin::ButtonClick&) {
		// rewin::CoordAnchor anchors[] = { rewin::CoordAnchor::BottomLeft };

		button->SetPos({ 0.5f, 0.5f, rewin::Relative, rewin::CoordAnchor(++ky) });

		return true;
	});
	*/

	window.Create();
	window.Show();

	while (window.Update())
		;
}
