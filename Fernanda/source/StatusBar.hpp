#pragma once

#include "common/Widget.hpp"

#include <QLabel>
#include <QStatusBar>
#include <QVector>

class StatusBar : public Widget<QStatusBar>
{
public:
	StatusBar(const char* name, QWidget* parent = nullptr)
		: Widget(name, parent)
	{
		setMaximumHeight(22);
	}

	void addWidgets(QWidgetList leftWidgets, QWidgetList rightWidgets)
	{
		for (auto& widget : leftWidgets) {
			if (widget == nullptr) continue;
			addPermanentWidget(widget, 0);
		}

		addPermanentWidget(m_spacer, 1);

		for (auto& widget : rightWidgets) {
			if (widget == nullptr) continue;
			addPermanentWidget(widget, 0);
		}
	}

	void addWidgets(QWidget* leftWidget, QWidget* rightWidget)
	{
		addWidgets({ leftWidget }, { rightWidget });
	}

	void addLeftWidget(QWidget* leftWidget)
	{
		addWidgets({ leftWidget }, QWidgetList{});
	}

	void addRightWidget(QWidget* rightWidget)
	{
		addWidgets(QWidgetList{}, { rightWidget });
	}

private:
	QLabel* m_spacer = new QLabel;
};
