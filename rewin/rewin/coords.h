#pragma once
#include <lm/lm.h>

namespace rewin
{
	enum CoordTypeTag
	{
		Absolute,
		Relative
	};

	enum class CoordAnchor
	{
		TopLeft,
		TopCenter,
		TopRight,
		LeftCenter,
		Center,
		RightCenter,
		BottomLeft,
		BottomCenter,
		BottomRight,
	};

	class Coords
	{
	public:
		Coords(float x, float y, CoordTypeTag type = Absolute, CoordAnchor anchor = CoordAnchor::BottomRight)
			: mValue{ x, y }, mAbsolute(type == Absolute), mCoordAnchor(anchor)
		{}

		Coords(const lm::Vec2& value, CoordTypeTag type = Absolute, CoordAnchor anchor = CoordAnchor::BottomRight)
			: mValue{ value }, mAbsolute(type == Absolute), mCoordAnchor(anchor)
		{}

		lm::Vec2 GetCoords(const lm::Vec2& scale) const
		{
			return mAbsolute ? mValue : mValue * scale;
		}

		lm::Vec2 GetAnchoredCoords(const lm::Vec2& scale, const lm::Vec2& size) const
		{
			auto pos = GetCoords(scale);

			switch (mCoordAnchor)
			{
			case CoordAnchor::TopLeft:
				return pos - size * lm::Vec2{ 1.f, 1.f };
			case CoordAnchor::TopCenter:
				return pos - size * lm::Vec2{ 0.5f, 1.f };
			case CoordAnchor::TopRight:
				return pos - size * lm::Vec2{ 0.f, 1.f };

			case CoordAnchor::BottomLeft:
				return pos - size * lm::Vec2{ 1.f, 0.f };
			case CoordAnchor::BottomCenter:
				return pos - size * lm::Vec2{ 0.5f, 0.f };
			case CoordAnchor::BottomRight:
				return pos - size * lm::Vec2{ 0.f, 0.f };

			case CoordAnchor::LeftCenter:
				return pos - size * lm::Vec2{ 1.f, 0.5f };
			case CoordAnchor::RightCenter:
				return pos - size * lm::Vec2{ 0.f, 0.5f };
			case CoordAnchor::Center:
				return pos - size * 0.5f;

			default:
				return pos;
			};
		}

		const lm::Vec2& GetRaw() const { return mValue; }
		bool IsAbsolute() const { return mAbsolute; }

	private:
		lm::Vec2 mValue;
		CoordAnchor mCoordAnchor;
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
