#include <rewin/window.h>
#include <rewin/button.h>
#include <rewin/label.h>
#include <rewin/image.h>
#include <rewin/text_box.h>
#include <rewin/list_box.h>

#include <rewin/blueprint.h>

#include <rewin/wm_events.h>

#include <CommCtrl.h>
#include <WinUser.h>

#pragma comment(lib, "comctl32.lib")

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

struct WindowCreated : rewin::Event<rewin::WindowMessageType::WindowCreated>
{
	int ky;
};

void PrintKy(rewin::Widget& widget, int tab = 0)
{
	for (auto i = 0; i < tab; i++)
		printf("\t");

	auto pos = widget.GetPos(), size = widget.GetSize();

	printf("- '%s' @ %p [%f, %f]-[%f, %f] visible=%s\n", widget.GetStringId().c_str(), &widget, pos.x, pos.y, size.x, size.y, widget.IsCurrentlyVisible() ? "true" : "false");

	for (auto& child : widget.GetChildren())
		PrintKy(*child, tab + 1);
}

namespace
{
	HFONT fontTahomaNormal = CreateFont(13, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, TEXT("Tahoma"));

	HFONT fontTahomaBold = CreateFont(13, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		FW_BOLD, TEXT("Tahoma"));

	HFONT fontPlakBig = CreateFont(36, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		FW_BOLD, TEXT("PF DinDisplay Pro"));

	HFONT fontTahomaItalics = CreateFont(13, 0, 0, 0, FW_DONTCARE, TRUE, FALSE, FALSE, ANSI_CHARSET,
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, TEXT("Tahoma"));

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
					rewin::WidgetBuilder<rewin::Label>(
						{ 0.5f, 0.2f, rewin::Relative, rewin::CoordAnchor::Center },
						{ 1.f, 0.1f, rewin::Relative },
						"PlakMP Launcher",
						rewin::XAlign::Center
					).SetFont(fontPlakBig)
				},
				{
					"NameInputHint",
					rewin::WidgetBuilder<rewin::Label>(
						{0.5f, 0.42f, rewin::Relative, rewin::CoordAnchor::Center},
						{250, 15},
						"Enter your PlakMP username:",
						rewin::XAlign::Left
					).SetFont(fontTahomaBold)
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
					rewin::WidgetBuilder<rewin::Label>(
						{0.5f, 0.58f, rewin::Relative, rewin::CoordAnchor::Center},
						{250, 15},
						"(3-32 characters!)",
						rewin::XAlign::Right
					).SetEnabled(false)
				},
				{
					"RunButton",
					rewin::Button(
						{ 0.5f, 0.8f, rewin::Relative, rewin::CoordAnchor::Center },
						{100, 40},
						"Run PlakMP"
					)
				},
			}
		}
	};

	rewin::Blueprint gPlakGraphicsClientWindow{
		{
			"MainArea",
			rewin::StaticControl(
				{ 0.f, 0.f, rewin::Relative },
				{ 1.f, 1.f, rewin::Relative },
				"", SS_CENTER
			),
			{
				rewin::BlueprintEntry{
					"PlakGraphicsLabel",
					rewin::WidgetBuilder<rewin::Label>(
						{ 0.5f, 0.2f, rewin::Relative, rewin::CoordAnchor::Center },
						{ 1.f, 0.2f, rewin::Relative },
						"Plak Graphics",
						rewin::XAlign::Center
					).SetFont(fontPlakBig)
				},
				rewin::BlueprintEntry{
					"LoadingArea",
					rewin::StaticControl(
						{ 0.f, 0.3f, rewin::Relative },
						{ 1.f, 1.f, rewin::Relative },
						"", SS_GRAYFRAME
					),
					{
						{
							"LoadingLabel",
							rewin::WidgetBuilder<rewin::Label>(
								{0.5f, 0.6f, rewin::Relative, rewin::CoordAnchor::Center},
								{1.f, 0.3f, rewin::Relative},
								"Loading content...",
								rewin::XAlign::Center
							).SetFont(fontTahomaBold)
						},
						{
							"ErrorLabel",
							rewin::Label(
								{0.5f, 0.7f, rewin::Relative, rewin::CoordAnchor::Center},
								{1.f, 0.3f, rewin::Relative},
								"Error loading content: Network request failed\nPlak Graphics may be unavailable at this time. Try again later.",
								rewin::XAlign::Center
							)
						},
						{
							"CancelButton",
							rewin::Button(
								{0.5f, 0.8f, rewin::Relative, rewin::CoordAnchor::Center},
								{100, 30},
								"Quit"
							)
						}
					}
				}
			}
		}
	};
}

int main()
{
	INITCOMMONCONTROLSEX commctrl;
	commctrl.dwSize = sizeof commctrl;
	commctrl.dwICC = 0xFFFFFFFF;

	InitCommonControlsEx(&commctrl);

	rewin::Window window(
		rewin::WindowClass{ "rewin-example" },
		{ 100, 200 }, { 500, 700 },
		"Rewin Example",
		rewin::WindowCreateMode::Defer
	);


	window.SetFont(fontTahomaNormal);

	rewin::Blueprint::LoadFromXML("D:/rewin/rewin-example/PlakGraphicsConfigurator.xml").LoadTo(&window);
	//gPlakGraphicsClientWindow.LoadTo(&window);

	/*
	testWindowContents.LoadTo(&window);

	int tab = 0;

	PrintKy(window, tab);

	auto pNameInput = window.FindChild<rewin::TextBox>("MainArea.NameInput");
	auto pRunButton = window.FindChild<rewin::Button>("MainArea.RunButton");
	auto pInputRules = window.FindChild<rewin::Label>("MainArea.NameInputRules");

	pNameInput->OnEvent<rewin::TextBoxContentChanged>([pNameInput, pRunButton, pInputRules, fontTahomaBold, fontTahomaNormal](const rewin::TextBoxContentChanged&) {
		auto size = pNameInput->GetText().size();
		auto ok = size > 3 && size < 32;

		pRunButton->SetEnabled(ok);

		pInputRules->SetEnabled(!ok);
		pInputRules->SetFont(ok ? fontTahomaNormal : fontTahomaBold);

		return true;
	});

	pRunButton->OnEvent<rewin::ButtonClick>([pNameInput, pRunButton](const rewin::ButtonClick&) {
		pNameInput->SetText("");

		return true;
	});
	*/

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

	window.FindChild<rewin::ListBox>("MainArea.ContentArea.ConfigArea.EditionsList")->AddItem("Plak Graphics 3.0\t\t\tRealistic Edition");
	window.FindChild<rewin::Button>("MainArea.ContentArea.ConfigArea.RunButton")->SetEnabled(false);

	PrintKy(window, 0);

	while (window.Update())
		;
}
