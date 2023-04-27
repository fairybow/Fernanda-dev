#pragma once

#include <QHBoxLayout>
#include <QMainWindow>
#include <QStackedLayout>
#include <QVBoxLayout>
#include <QVector>

namespace Layout
{
	enum class Line {
		Horizontally,
		Vertically
	};

	namespace {
		inline void setBoxProperties(QBoxLayout* box, QVector<QWidget*> widgets)
		{
			box->setContentsMargins(0, 0, 0, 0);
			box->setSpacing(0);
			for (auto& widget : widgets)
				box->addWidget(widget);
		}

		inline void setStackProperties(QStackedLayout* stack, QVector<QWidget*> widgets)
		{
			stack->setContentsMargins(0, 0, 0, 0);
			stack->setSpacing(0);
			stack->setStackingMode(QStackedLayout::StackAll);
			for (auto& widget : widgets)
				stack->addWidget(widget);
		}
	}

	inline void box(QWidget* parent, QVector<QWidget*> widgets, Line alignment = Line::Vertically)
	{
		QBoxLayout* layout = nullptr;
		(alignment == Line::Horizontally)
			? layout = new QHBoxLayout(parent)
			: layout = new QVBoxLayout(parent);
		setBoxProperties(layout, widgets);
		parent->setLayout(layout);
	}

	inline void box(QWidget* parent, QWidget* widget, Line alignment = Line::Vertically) { box(parent, QVector<QWidget*>{ widget }, alignment); }

	inline void setCentralWidgets(QMainWindow* parentWindow, QVector<QWidget*> widgets)
	{
		auto container = new QWidget(parentWindow);
		auto layout = new QStackedLayout(container);
		setStackProperties(layout, widgets);
		parentWindow->setCentralWidget(container);
	}

	inline void setCentralWidget(QMainWindow* parentWindow, QWidget* widget) { setCentralWidgets(parentWindow, QVector<QWidget*>{ widget }); }

	inline void stack(QWidget* parent, QVector<QWidget*> widgets)
	{
		auto layout = new QStackedLayout(parent);
		setStackProperties(layout, widgets);
		parent->setLayout(layout);
	}

	inline void stack(QWidget* parent, QWidget* widget) { stack(parent, QVector<QWidget*>{ widget }); }
}
