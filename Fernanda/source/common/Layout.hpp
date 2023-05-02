#pragma once

#include <QHBoxLayout>
#include <QLabel>
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

	namespace
	{
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

	inline QBoxLayout* box(QWidget* parent, QVector<QWidget*> widgets = {}, Line alignment = Line::Vertically)
	{
		QBoxLayout* layout = nullptr;
		(alignment == Line::Horizontally)
			? layout = new QHBoxLayout(parent)
			: layout = new QVBoxLayout(parent);
		setBoxProperties(layout, widgets);
		parent->setLayout(layout);
		return layout;
	}

	inline QBoxLayout* box(QWidget* parent, QWidget* widget, Line alignment = Line::Vertically)
	{
		return box(parent, QVector<QWidget*>{ widget }, alignment);
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

	inline QWidget* labeledContainer(const QString& text, QVector<QWidget*> widgets, QWidget* parent = nullptr)
	{
		auto container = new QWidget;

		//auto label = QLabel;
		/*auto layout = new QVBoxLayout(container);
		auto label = new QLabel(container);
		label->setText(text);
		layout->addWidget(label);
		layout->addWidget(comboBox);
		}*/
		return container;
	}

	inline QWidget* labeledContainer(const QString& text, QWidget* widget, QWidget* parent = nullptr)
	{
		return labeledContainer(text, QVector<QWidget*>{ widget }, parent);
	}
}
