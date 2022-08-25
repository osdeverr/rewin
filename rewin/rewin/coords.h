#pragma once
#include <lm/lm.h>

namespace rewin
{
	inline static constexpr enum AbsoluteCoordTag {} Absolute{0};
	inline static constexpr enum RelativeCoordTag {} Relative{0};

	class Coords
	{
	public:
		Coords(float x, float y, AbsoluteCoordTag = Absolute)
			: mValue{ x, y }, mAbsolute(true)
		{}

		Coords(float x, float y, RelativeCoordTag)
			: mValue{ x, y }, mAbsolute(false)
		{}

		Coords(const lm::Vec2& value, AbsoluteCoordTag = Absolute)
			: mValue{ value }, mAbsolute(true)
		{}

		Coords(const lm::Vec2& value, RelativeCoordTag)
			: mValue{ value }, mAbsolute(false)
		{}

		lm::Vec2 GetCoords(const lm::Vec2& scale)
		{
			return mAbsolute ? mValue : mValue * scale;
		}

	private:
		lm::Vec2 mValue;
		bool mAbsolute;
	};
}
