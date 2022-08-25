#pragma once
#include "widget.h"

namespace rewin
{
	class Control : public Widget
	{
	public:
		Control(
			const Coords& pos, const Coords& size,
			const std::string& classId,
			uint32_t style
		);

		virtual void Activate(Widget* pParent, int id) override;

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
