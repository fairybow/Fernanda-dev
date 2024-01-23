#pragma once

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

		inline void setProperties(QLayout* layout, QMargins margins)
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

	enum class Box {
		Horizontal,
		Vertical
	};

	inline QBoxLayout* box(Box type, QWidget* parent = nullptr, QObjectList objects = {}, QMargins margins = QMargins())
	{
		QBoxLayout* layout = nullptr;
		(type == Layout::Box::Horizontal)
			? layout = new QHBoxLayout
			: layout = new QVBoxLayout;

		setProperties(layout, margins);
		addObjects(layout, objects);

		if (parent && !parent->layout())
			parent->setLayout(layout);

		return layout;
	}

	inline QBoxLayout* box(Box type, QObjectList objects = {}, QMargins margins = QMargins())
	{
		return box(type, nullptr, objects, margins);
	}

	inline QBoxLayout* box(Box type, QWidget* parent = nullptr, QWidgetList widgets = {}, QMargins margins = QMargins())
	{
		QObjectList objects;

		for (auto& widget : widgets)
			objects << widget;

		return box(type, parent, objects, margins);
	}

	inline QBoxLayout* box(Box type, QWidgetList widgets = {}, QMargins margins = QMargins())
	{
		return box(type, nullptr, widgets, margins);
	}
}
