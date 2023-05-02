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
	enum class Line {
		Horizontally,
		Vertically
	};

	namespace
	{
		inline void setBoxProperties(QBoxLayout* box, QVector<QWidget*> widgets, QMargins margins)
		{
			box->setContentsMargins(margins);
			box->setSpacing(0);
			for (auto& widget : widgets)
				box->addWidget(widget);
		}

		inline void setStackProperties(QStackedLayout* stack, QVector<QWidget*> widgets)
		{
			stack->setContentsMargins(QMargins());
			stack->setSpacing(0);
			stack->setStackingMode(QStackedLayout::StackAll);
			for (auto& widget : widgets)
				stack->addWidget(widget);
		}
	}

	inline QBoxLayout* box(QVector<QWidget*> widgets = {}, QWidget* parent = nullptr,
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
		return box(QVector<QWidget*>{ widget }, parent, alignment, margins);
	}

	inline void setCentralWidgets(QMainWindow* parentWindow, QVector<QWidget*> widgets)
	{
		auto container = new QWidget(parentWindow);
		auto layout = new QStackedLayout(container);
		setStackProperties(layout, widgets);
		parentWindow->setCentralWidget(container);
	}

	inline void setCentralWidget(QMainWindow* parentWindow, QWidget* widget)
	{
		setCentralWidgets(parentWindow, QVector<QWidget*>{ widget });
	}

	inline QStackedLayout* stack(QVector<QWidget*> widgets, QWidget* parent = nullptr)
	{
		auto layout = new QStackedLayout(parent);
		setStackProperties(layout, widgets);
		if (parent != nullptr)
			parent->setLayout(layout);
		return layout;
	}

	inline QStackedLayout* stack(QWidget* widget, QWidget* parent = nullptr)
	{
		return stack(QVector<QWidget*>{ widget }, parent);
	}

	inline QWidget* labeledContainer(const QString& text, QVector<QWidget*> widgets,
		QWidget* parent = nullptr, Line alignment = Line::Vertically, QMargins margins = { 10, 10, 10, 10 })
	{
		auto container = new QWidget(parent);
		auto label = new QLabel;
		label->setText(text);
		QVector<QWidget*> amended_widgets = { label };
		amended_widgets << widgets;
		auto layout = box(amended_widgets, container, alignment, margins);
		return container;
	}

	inline QWidget* labeledContainer(const QString& text, QWidget* widget,
		QWidget* parent = nullptr, Line alignment = Line::Vertically, QMargins margins = { 10, 10, 10, 10 })
	{
		return labeledContainer(text, QVector<QWidget*>{ widget }, parent, alignment, margins);
	}
}
