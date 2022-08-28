#pragma once
#include "widget.h"

namespace rewin
{
	enum ControlFlags
	{
		ControlFlags_Adjustable = 1 << 0,
		ControlFlags_StickToBottom = 1 << 1,
		ControlFlags_StickToLeft = 1 << 2,
		ControlFlags_NoDivider = 1 << 3,
		ControlFlags_NoMoveX = 1 << 4,
		ControlFlags_NoMoveY = 1 << 5,
		ControlFlags_NoParentAlign = 1 << 6,
		ControlFlags_NoResize = 1 << 7,
		ControlFlags_StickToRight = 1 << 8,
		ControlFlags_StickToTop = 1 << 9,
		ControlFlags_Vertical = 1 << 10
	};

	class Control : public Widget
	{
	public:
		Control(
			const Coords& pos, const Coords& size,
			const std::string& classId,
			uint32_t style
		);

		virtual void Activate(Widget* pParent, int id) override;
		virtual void RecalculateSize() override;

		int GetControlId() const { return mControlId; }

		void SetText(const std::string& text);

	protected:
		int mControlId = -1;
		std::string mText = "ky";
		std::string mClassId;
		uint32_t mStyle;
	};

	template<auto T, auto NC>
	struct ControlEvent : Event<T>
	{
		static bool ShouldProcess(Widget* widget, WindowParam w, WindowParam) { return (w & 0xFFFF) == ((Control*)widget)->GetControlId() && ((w & 0xFFFF0000) >> 16) == NC; }

		ControlEvent()
		{}

		ControlEvent(WindowParam, WindowParam)
		{}
	};
}
