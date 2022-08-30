#include "blueprint.h"

namespace rewin
{
	Widget* BlueprintEntry::Create() const
	{
		Widget* pWidget = cloneFunction(this->pWidget);
		pWidget->KillChildren();

		for (auto& child : children)
			pWidget->Add(child.Create());

		return pWidget;
	}

	BlueprintEntry* BlueprintEntry::FindChild(const std::vector<std::string>& path, int skip)
	{
		if (skip >= path.size())
			return nullptr;

		for (auto& child : children)
			if (child.id == path[skip])
				if (skip == path.size() - 1)
					return &child;
				else
					return child.FindChild(path, skip);

		return nullptr;
	}

	void Blueprint::LoadTo(Widget* pWidget)
	{
		for (auto& child : mChildren)
			pWidget->Add(child.Create());
	}

	Widget* Blueprint::FindWidget(const std::vector<std::string>& path)
	{
		for (auto& child : mChildren)
			if (child.id == path[0])
				if (auto pEntry = child.FindChild(path, 1))
					return pEntry->pWidget;

		return nullptr;
	}
}
