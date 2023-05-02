#pragma once

#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QMargins>
#include <QStackedLayout>
#include <QVBoxLayout>
#include <QVector>

namespace Layout
{
	enum class Line { Horizontally, Vertically };
	enum class Type { Box, Stack };

	namespace
	{
		inline void add(QLayout* layout, QWidgetList widgets)
		{
			for (auto& widget : widgets) {
				if (widget == nullptr) continue;
				layout->addWidget(widget);
			}
		}

		inline void setBoxProperties(QBoxLayout* box, QWidgetList widgets, QMargins margins)
		{
			box->setContentsMargins(margins);
			box->setSpacing(0);
			add(box, widgets);
		}

		inline void setStackProperties(QStackedLayout* stack, QWidgetList widgets)
		{
			stack->setContentsMargins(QMargins());
			stack->setSpacing(0);
			stack->setStackingMode(QStackedLayout::StackAll);
			add(stack, widgets);
		}
	}

	inline QBoxLayout* box(QWidgetList widgets = {}, QWidget* parent = nullptr,
		Line alignment = Line::Vertically, QMargins margins = QMargins())
	{
		QBoxLayout* layout = nullptr;
		(alignment == Line::Horizontally)
			? layout = new QHBoxLayout(parent)
			: layout = new QVBoxLayout(parent);
		setBoxProperties(layout, widgets, margins);
		if (parent)
			parent->setLayout(layout);
		return layout;
	}

	inline QBoxLayout* box(QWidget* widget, QWidget* parent = nullptr,
		Line alignment = Line::Vertically, QMargins margins = QMargins())
	{
		return box(QWidgetList{ widget }, parent, alignment, margins);
	}

	inline QStackedLayout* stack(QWidgetList widgets, QWidget* parent = nullptr)
	{
		auto layout = new QStackedLayout(parent);
		setStackProperties(layout, widgets);
		if (parent != nullptr)
			parent->setLayout(layout);
		return layout;
	}

	inline QStackedLayout* stack(QWidget* widget, QWidget* parent = nullptr)
	{
		return stack(QWidgetList{ widget }, parent);
	}

	inline void setCentralWidgets(QMainWindow* parentWindow, QWidgetList widgets, Type type = Type::Stack, Line alignment = Line::Vertically)
	{
		auto container = new QWidget(parentWindow);
		QLayout* layout = nullptr;
		(type == Type::Stack)
			? layout = stack(widgets, container)
			: layout = box(widgets, container, alignment);
		parentWindow->setCentralWidget(container);
	}

	inline void setCentralWidget(QMainWindow* parentWindow, QWidget* widget, Type type = Type::Stack, Line alignment = Line::Vertically)
	{
		setCentralWidgets(parentWindow, QWidgetList{ widget }, type, alignment);
	}

	inline QWidget* labeledContainer(const QString& text, QWidgetList widgets,
		QWidget* parent = nullptr, Line alignment = Line::Vertically, QMargins margins = { 10, 10, 10, 10 })
	{
		auto container = new QWidget(parent);
		auto label = new QLabel;
		label->setText(text);
		QWidgetList amended_widgets = { label };
		amended_widgets << widgets;
		auto layout = box(amended_widgets, container, alignment, margins);
		return container;
	}

	inline QWidget* labeledContainer(const QString& text, QWidget* widget,
		QWidget* parent = nullptr, Line alignment = Line::Vertically, QMargins margins = { 10, 10, 10, 10 })
	{
		return labeledContainer(text, QWidgetList{ widget }, parent, alignment, margins);
	}
}
