#pragma once

#include <QMainWindow>
#include <QStackedLayout>
#include <QVBoxLayout>
#include <QVector>

namespace Layout
{
	inline void setVBoxLayout(QWidget* parent, QVector<QWidget*> widgets)
	{
		auto layout = new QVBoxLayout(parent);
		layout->setContentsMargins(0, 0, 0, 0);
		layout->setSpacing(0);
		for (auto& widget : widgets)
			layout->addWidget(widget);
		parent->setLayout(layout);
	}

	inline void setVBoxLayout(QWidget* parent, QWidget* widget) { setVBoxLayout(parent, QVector<QWidget*>{ widget }); }

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

	inline void setStack(QWidget* parent, QVector<QWidget*> widgets)
	{
		auto stack_layout = new QStackedLayout(parent);
		stack_layout->setStackingMode(QStackedLayout::StackAll);
		for (auto& widget : widgets)
			stack_layout->addWidget(widget);
		parent->setLayout(stack_layout);
	}

	inline void set(QWidget* parent, QWidget* widget) { setStack(parent, QVector<QWidget*>{ widget }); }
}
