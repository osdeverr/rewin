#pragma once
#include "widget.h"

#include <ulib/list.h>

namespace rewin
{
	struct BlueprintEntry
	{
		std::string id;
		Widget* pWidget;

		std::function<Widget* (Widget*)> cloneFunction;

		std::vector<BlueprintEntry> children;

		template<class T>
		BlueprintEntry(const T& widget)
			: id{ "" }, pWidget{ new T(widget) }
		{
			cloneFunction = [](Widget* pWidget) { return new T(*(T*)pWidget); };
		}

		template<class T>
		BlueprintEntry(const T& widget, std::initializer_list<BlueprintEntry> children)
		{
			cloneFunction = [](Widget* pWidget) { return new T(*(T*)pWidget); };
		}

		template<class T>
		BlueprintEntry(const std::string& id, const T& widget)
			: id{ id }, pWidget{ new T(widget) }
		{
			cloneFunction = [](Widget* pWidget) { return new T(*(T*)pWidget); };
		}

		template<class T>
		BlueprintEntry(const std::string& id, const T& widget, std::initializer_list<BlueprintEntry> children)
			: id{ id }, pWidget{ new T(widget) }, children{ children }
		{
			cloneFunction = [](Widget* pWidget) { return new T(*(T*)pWidget); };
		}

		Widget* Create() const;

		BlueprintEntry(const BlueprintEntry& other)
		{
			id = other.id;
			pWidget = other.cloneFunction(other.pWidget);
			cloneFunction = other.cloneFunction;
			children = other.children;
		}

		~BlueprintEntry()
		{
			delete pWidget;
		}
	};

	class Blueprint
	{
	public:
		Blueprint(std::initializer_list<BlueprintEntry> children)
			: mChildren{ children }
		{}

		void LoadTo(Widget* pWidget);

	private:
		std::vector<BlueprintEntry> mChildren;
	};
}
