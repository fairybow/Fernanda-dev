#pragma once

#include "common/Path.hpp"
#include "common/Widget.hpp"

#include <QSize>
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
		//setAutoHide(true);
		//setTabsClosable(true);
		setMovable(true);
		setExpanding(false);
	}

	int find(QUuid id, StdFsPath pathForTitle = StdFsPath(), bool switchTo = true)
	{
		auto index = -1;
		for (auto i = 0; i < count(); ++i) {
			if (tabData(i) == id) {
				index = i;
				break;
			}
		}
		if (index == -1) {
			blockSignals(true);
			index = addTab(
				pathForTitle.empty() ? QString() : Path::qStringName(pathForTitle));
			setTabData(index, id);
			blockSignals(false);
		}
		if (switchTo)
			setCurrentIndex(index);
		return index;
	}

	QUuid id(int index)
	{
		return tabData(index).value<QUuid>();
	}

protected:
	/*virtual QSize minimumTabSizeHint(int index) const override
	{
		return QSize(100, QTabBar::minimumTabSizeHint(index).height());
	}*/
};
