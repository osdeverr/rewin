#include <rewin/widget.h>
#include <rewin/coords.h>

struct WindowCreated : rewin::Event<rewin::WindowMessageType::WindowCreated>
{
	int ky;
};

int main()
{
	rewin::Widget widget;

	rewin::Coords coord = { 5, 6, rewin::Relative };

	widget.OnEvent<WindowCreated>([](const WindowCreated& event) {

	});
}
