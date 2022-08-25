#pragma once
#include "widget.h"
#include <string>

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

	class Window : public Widget
	{
	public:
		Window(
			const WindowClass& windowClass,
			const lm::Vec2& pos, const lm::Vec2& size,
			const std::string& name,
			WindowCreateMode mode = WindowCreateMode::Now
		);

		void Create();
		void Show(bool show = true);

		bool Update();

		const std::string& GetName() const
		{
			return mName;
		}

	private:
		WindowClass mWindowClass;
		std::string mName;
	};
}
