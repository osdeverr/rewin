#include "blueprint.h"

#include <pugixml/pugixml.hpp>

#include "window.h"
#include "static_control.h"
#include "label.h"
#include "button.h"
#include "text_box.h"

#include <algorithm>

namespace rewin
{
	namespace
	{
		struct IXmlFactory
		{
			virtual ~IXmlFactory() = default;

			virtual BlueprintEntry CreateBlueprintEntry(const pugi::xml_node& node) = 0;
		};

		extern std::unordered_map<std::string, IXmlFactory*> sWidgetFactories;

		BlueprintEntry CreateXmlBlueprintEntry(const pugi::xml_node& node)
		{
			BlueprintEntry entry = sWidgetFactories.at(node.name())->CreateBlueprintEntry(node);

			if (!node.text())
			{
				for (auto& child : node.children())
					entry.children.push_back(CreateXmlBlueprintEntry(child));
			}

			return entry;
		}

		std::pair<pugi::xml_attribute, bool> FindDescendableAttribute(const pugi::xml_node& node, const char* name, bool first = true)
		{
			if (auto attr = node.attribute(name))
				return std::make_pair(attr, first);
			else if (auto parent = node.parent())
				return FindDescendableAttribute(parent, name, first);
			else
				return {};
		}

		template<class T>
		T& LoadCommonWidgetData(T& widget, const pugi::xml_node& node)
		{
			auto [fontFamily, fontFamilyFirst] = FindDescendableAttribute(node, "fontFamily");
			auto [fontSize, fontSizeFirst] = FindDescendableAttribute(node, "fontSize");
			auto [fontWeight, fontWeightFirst] = FindDescendableAttribute(node, "fontWeight");

			if (fontFamilyFirst || fontSizeFirst || fontWeightFirst)
			{
				auto font = CreateFontA(fontSize.as_int(13), 0, 0, 0, fontWeight.as_int(FW_DONTCARE), FALSE, FALSE, FALSE, ANSI_CHARSET,
					OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
					DEFAULT_PITCH | FF_DONTCARE, fontFamily.as_string("Tahoma"));

				widget.SetFont(font);
			}

			return widget;
		}

		CoordAnchor LoadCoordAnchor(const pugi::xml_node& node)
		{
			std::string s = node.attribute("anchor").as_string("BottomRight");

			std::transform(s.begin(), s.end(), s.begin(),
				[](unsigned char c) { return std::tolower(c); });

			if (s == "topleft")
				return CoordAnchor::TopLeft;
			else if (s == "topcenter")
				return CoordAnchor::TopCenter;
			else if (s == "topright")
				return CoordAnchor::TopRight;
			else if (s == "leftcenter")
				return CoordAnchor::LeftCenter;
			else if (s == "center")
				return CoordAnchor::Center;
			else if (s == "rightcenter")
				return CoordAnchor::RightCenter;
			else if (s == "bottomleft")
				return CoordAnchor::BottomLeft;
			else if (s == "bottomcenter")
				return CoordAnchor::BottomCenter;
			else if (s == "bottomright")
				return CoordAnchor::BottomRight;
			else
				return CoordAnchor::BottomRight;
		}

		XAlign LoadXAlign(const pugi::xml_node& node)
		{
			std::string s = node.attribute("xAlign").as_string("left");

			std::transform(s.begin(), s.end(), s.begin(),
				[](unsigned char c) { return std::tolower(c); });

			if (s == "left")
				return XAlign::Left;
			else if (s == "center")
				return XAlign::Center;
			else if (s == "right")
				return XAlign::Right;
		}

		Coords LoadPosition(const pugi::xml_node& node)
		{
			std::string text = node.attribute("pos").as_string("0,0");
			CoordTypeTag type = Absolute;

			if (text.back() == 'r')
			{
				text.pop_back();
				type = Relative;
			}

			std::istringstream ss{ text };

			float x, y;

			ss >> x;

			while (ss.peek() == ',' || ss.peek() == ' ')
				ss.ignore();

			ss >> y;

			return Coords{ x, y, type, LoadCoordAnchor(node) };
		}

		Coords LoadSize(const pugi::xml_node& node)
		{
			std::string text = node.attribute("size").as_string("0,0");
			CoordTypeTag type = Absolute;

			if (text.back() == 'r')
			{
				text.pop_back();
				type = Relative;
			}

			std::istringstream ss{ text };

			float x, y;

			ss >> x;

			while (ss.peek() == ',' || ss.peek() == ' ')
				ss.ignore();

			ss >> y;

			return Coords{ x, y, type };
		}

		struct WindowXmlFactory : public IXmlFactory
		{
			virtual BlueprintEntry CreateBlueprintEntry(const pugi::xml_node& node)
			{
				return BlueprintEntry{
					node.attribute("id").as_string(),
					LoadCommonWidgetData(Window(
						WindowClass(node.attribute("class").as_string()),
						LoadPosition(node).GetRaw(),
						LoadSize(node).GetRaw(),
						node.attribute("title").as_string(),
						rewin::WindowCreateMode::Defer
					), node)
				};
			}
		};

		struct StaticControlXmlFactory : public IXmlFactory
		{
			virtual BlueprintEntry CreateBlueprintEntry(const pugi::xml_node& node)
			{
				return BlueprintEntry{
					node.attribute("id").as_string(),
					LoadCommonWidgetData(StaticControl(
						LoadPosition(node),
						LoadSize(node),
						node.attribute("text").as_string(node.text().as_string()),
						SS_CENTER
					), node)
				};
			}
		};

		struct LabelXmlFactory : public IXmlFactory
		{
			virtual BlueprintEntry CreateBlueprintEntry(const pugi::xml_node& node)
			{
				return BlueprintEntry{
					node.attribute("id").as_string(),
					LoadCommonWidgetData(Label(
						LoadPosition(node),
						LoadSize(node),
						node.attribute("text").as_string(node.text().as_string()),
						LoadXAlign(node)
					), node)
				};
			}
		};

		struct ButtonXmlFactory : public IXmlFactory
		{
			virtual BlueprintEntry CreateBlueprintEntry(const pugi::xml_node& node)
			{
				return BlueprintEntry{
					node.attribute("id").as_string(),
					LoadCommonWidgetData(Button(
						LoadPosition(node),
						LoadSize(node),
						node.attribute("text").as_string(node.text().as_string())
					), node)
				};
			}
		};

		struct TextBoxXmlFactory : public IXmlFactory
		{
			virtual BlueprintEntry CreateBlueprintEntry(const pugi::xml_node& node)
			{
				return BlueprintEntry{
					node.attribute("id").as_string(),
					LoadCommonWidgetData(TextBox(
						LoadPosition(node),
						LoadSize(node),
						node.attribute("text").as_string(node.text().as_string()),
						{}
					), node)
				};
			}
		};

		std::unordered_map<std::string, IXmlFactory*> sWidgetFactories = {
			{"Window", new WindowXmlFactory()},
			{"Static", new StaticControlXmlFactory()},
			{"Area", new StaticControlXmlFactory()},
			{"Label", new LabelXmlFactory()},
			{"Button", new ButtonXmlFactory()},
			{"TextBox", new TextBoxXmlFactory()}
		};
	}

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
					return child.FindChild(path, skip + 1);

		return nullptr;
	}

	Blueprint Blueprint::LoadFromXML(const std::string& path)
	{
		pugi::xml_document doc;
		assert(doc.load_file(path.c_str()));

		auto blueprint = Blueprint{};

		for (auto& child : doc.children())
			blueprint.mChildren.push_back(CreateXmlBlueprintEntry(child));

		return blueprint;
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
