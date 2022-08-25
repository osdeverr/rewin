#pragma once
#include "control.h"

namespace rewin
{
	class StaticControl : public Control
	{
	public:
		StaticControl(
			const Coords& pos, const Coords& size,
			const std::string& text,
			uint32_t exStyle
		);
	};
}
