#pragma once

#include "common/Path.hpp"
#include "common/Widget.hpp"

#include <QTabBar>
#include <QUuid>

#include <filesystem>

class TabBar : public Widget<QTabBar>
{
	using StdFsPath = std::filesystem::path;

public:
	TabBar(const char* name, QWidget* parent = nullptr)
		: Widget(name, parent)
	{
		setAutoHide(true);
	}

	void add(QUuid id, StdFsPath path = StdFsPath())
	{
		// add based on uuid received from Document::create()/find()
		// optional path for name only
		// if no name, name dynamically created from first text block on keystroke
		blockSignals(true);
		QString text = path.empty() ? "" : Path::qStringName(path);
		auto index = addTab(text);
		setTabData(index, id);
		blockSignals(false);
	}

	int find(QUuid id, bool switchTo = true)
	{
		auto index = -1;
		for (auto i = 0; i < count(); ++i) {
			if (tabData(i) == id) {
				index = i;
				break;
			}
		}
		if (index < 0)
			index = 0;
		if (switchTo)
			setCurrentIndex(index);
		return index;
	}
};
