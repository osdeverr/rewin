#include "blueprint.h"

namespace rewin
{
	Widget* BlueprintEntry::Create() const
	{
		Widget* pWidget = cloneFunction(this->pWidget);

		for (auto& child : children)
			pWidget->Add(child.Create());

		return pWidget;
	}

	void Blueprint::LoadTo(Widget* pWidget)
	{
		for (auto& child : mChildren)
			pWidget->Add(child.Create());
	}
}
