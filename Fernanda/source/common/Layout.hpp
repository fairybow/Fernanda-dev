#pragma once

#include <QGridLayout>
#include <QHBoxLayout>
#include <QList>
#include <QMargins>
#include <QVBoxLayout>
#include <QWidget>

namespace Layout
{
	namespace
	{
		inline QWidget* toWidget(QObject* object) { return qobject_cast<QWidget*>(object); }
		inline QLayout* toSublayout(QObject* object) { return qobject_cast<QLayout*>(object); }

		inline bool isWidget(QObject* object) { return toWidget(object); }
		inline bool isSublayout(QObject* object) { return toSublayout(object); }

		inline void setProperties(QLayout* layout, const QMargins& margins)
		{
			layout->setContentsMargins(margins);
			layout->setSpacing(0);
		}

		inline void addObjects(QLayout* layout, QObjectList objects)
		{
			for (auto& object : objects)
				if (isWidget(object))
					layout->addWidget(toWidget(object));
				else if (isSublayout(object)) {
					layout->addItem(toSublayout(object));

					if (!object->parent())
						object->setParent(layout);
				}
		}
	}

	enum class Orientation {
		Horizontal,
		Vertical
	};

	inline QBoxLayout* box(Orientation orientation, QWidget* parent = nullptr, QObjectList objects = {}, const QMargins& margins = QMargins())
	{
		QBoxLayout* layout = nullptr;
		(orientation == Orientation::Horizontal)
			? layout = new QHBoxLayout
			: layout = new QVBoxLayout;

		setProperties(layout, margins);
		addObjects(layout, objects);

		if (parent && !parent->layout())
			parent->setLayout(layout);

		return layout;
	}

	inline QBoxLayout* box(Orientation orientation, QObjectList objects = {}, const QMargins& margins = QMargins())
	{
		return box(orientation, nullptr, objects, margins);
	}

	inline QBoxLayout* box(Orientation orientation, QWidget* parent = nullptr, QWidgetList widgets = {}, const QMargins& margins = QMargins())
	{
		QObjectList objects;

		for (auto& widget : widgets)
			objects << widget;

		return box(orientation, parent, objects, margins);
	}

	inline QBoxLayout* box(Orientation orientation, QWidgetList widgets = {}, const QMargins& margins = QMargins())
	{
		return box(orientation, nullptr, widgets, margins);
	}

	inline void evenlyAddToGrid(QGridLayout* grid, QWidgetList widgets, int maxColumnsOrRows = 4, Orientation orientation = Orientation::Horizontal)
	{
		if (maxColumnsOrRows < 1)
			maxColumnsOrRows = 1;

		auto row = 0;
		auto column = 0;

		for (auto& widget : widgets) {
			if (orientation == Orientation::Horizontal) {
				grid->addWidget(widget, row, column++);
				if (column == maxColumnsOrRows) {
					column = 0;
					row++;
				}
			}
			else {
				grid->addWidget(widget, row++, column);
				if (row == maxColumnsOrRows) {
					row = 0;
					column++;
				}
			}
		}
	}
}
