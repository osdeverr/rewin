#pragma once
#include "static_control.h"

namespace rewin
{
	class Label : public StaticControl
	{
	public:
		Label(
			const Coords& pos, const Coords& size,
			const std::string& text,
			XAlign textAlign = XAlign::Left
		);
	};
}
