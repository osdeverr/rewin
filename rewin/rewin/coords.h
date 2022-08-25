#pragma once
#include <lm/lm.h>

namespace rewin
{
	inline static constexpr enum class AbsoluteCoordTag { _ } Absolute = AbsoluteCoordTag::_;
	inline static constexpr enum class RelativeCoordTag { _ } Relative = RelativeCoordTag::_;

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

		lm::Vec2 GetCoords(const lm::Vec2& scale) const
		{
			return mAbsolute ? mValue : mValue * scale;
		}

		const lm::Vec2& GetRaw() const { return mValue; }
		bool IsAbsolute() const { return mAbsolute; }

	private:
		lm::Vec2 mValue;
		bool mAbsolute;
	};

	enum class XAlign
	{
		Left,
		Right,
		Center,
	};

	enum class YAlign
	{
		Top,
		Bottom,
		Center
	};
}
