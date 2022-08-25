#include <rewin/widget.h>
#include <rewin/coords.h>
#include <rewin/window.h>

#include <rewin/wm_events.h>

struct WindowCreated : rewin::Event<rewin::WindowMessageType::WindowCreated>
{
	int ky;
};

int main()
{
	rewin::Window window(
		rewin::WindowClass{ "rewin-example" },
		{ 100, 200 }, { 1000, 500 },
		"Rewin Example",
		rewin::WindowCreateMode::Defer
	);

	window.OnEvent<rewin::WindowCreate>([](const rewin::WindowCreate& event) {
		printf("Window '%s' Created\n", event.pWindow->GetName().c_str());
	});

	window.Create();
	window.Show();

	while (window.Update())
		;
}
