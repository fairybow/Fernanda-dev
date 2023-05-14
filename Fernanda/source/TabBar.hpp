#pragma once

#include "common/Path.hpp"
#include "common/Widget.hpp"

#include <QString>
#include <QTabBar>

class TabBar : public Widget<QTabBar>
{
public:
	TabBar(const char* name, QWidget* parent = nullptr)
		: Widget(name, parent)
	{
		setAutoHide(true);
	}

	int findOrAdd(const QString& path, bool switchTo = true)
	{
		auto index = -1;
		for (auto i = 0; i < count(); ++i) {
			if (tabData(i).toString() == path) {
				index = i;
				break;
			}
		}
		if (index == -1) {
			blockSignals(true);
			index = addTab(Path::qStringName(path));
			setTabData(index, path);
			blockSignals(false);
		}
		if (switchTo)
			setCurrentIndex(index);
		return index;
	}
};
