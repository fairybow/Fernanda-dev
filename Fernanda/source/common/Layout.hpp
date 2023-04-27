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

	inline void box(QWidget* parent, QVector<QWidget*> widgets, Line alignment = Line::Vertically)
	{
		QBoxLayout* layout = nullptr;
		(alignment == Line::Horizontally)
			? layout = new QHBoxLayout(parent)
			: layout = new QVBoxLayout(parent);
		layout->setContentsMargins(0, 0, 0, 0);
		layout->setSpacing(0);
		for (auto& widget : widgets)
			layout->addWidget(widget);
		parent->setLayout(layout);
	}

	inline void box(QWidget* parent, QWidget* widget, Line alignment = Line::Vertically) { box(parent, QVector<QWidget*>{ widget }, alignment); }

	inline void setCentralWidgets(QMainWindow* parentWindow, QVector<QWidget*> widgets)
	{
		auto container = new QWidget(parentWindow);
		auto stack_layout = new QStackedLayout(container);
		stack_layout->setStackingMode(QStackedLayout::StackAll);
		for (auto& widget : widgets)
			stack_layout->addWidget(widget);
		parentWindow->setCentralWidget(container);
	}

	inline void setCentralWidget(QMainWindow* parentWindow, QWidget* widget) { setCentralWidgets(parentWindow, QVector<QWidget*>{ widget }); }

	inline void stack(QWidget* parent, QVector<QWidget*> widgets)
	{
		auto stack_layout = new QStackedLayout(parent);
		stack_layout->setStackingMode(QStackedLayout::StackAll);
		for (auto& widget : widgets)
			stack_layout->addWidget(widget);
		parent->setLayout(stack_layout);
	}

	inline void set(QWidget* parent, QWidget* widget) { stack(parent, QVector<QWidget*>{ widget }); }
}
