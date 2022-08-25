#include "label.h"
#include <Windows.h>

namespace rewin
{
	static uint32_t TextAlignToStyle(XAlign align)
	{
		switch (align)
		{
		case XAlign::Left:
			return SS_LEFT;
		case XAlign::Center:
			return SS_CENTER;
		case XAlign::Right:
			return SS_RIGHT;

		default:
			return 0;
		}
	}

	Label::Label(
		const Coords& pos, const Coords& size,
		const std::string& text,
		XAlign textAlign
	) : StaticControl{ pos, size, text, TextAlignToStyle(textAlign) }
	{}
}
