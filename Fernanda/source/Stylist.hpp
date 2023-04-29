#pragma once

#include "common/Io.hpp"
#include "common/Path.hpp"

#include <QPair>
#include <QVector>
#include <QWidget>

class Stylist : public QObject
{
	using StdFsPath = std::filesystem::path;

public:
	inline Stylist(QVector<QWidget*> widgets, QObject* parent = nullptr)
		: QObject(parent)
	{
		for (auto& widget : widgets)
			add(widget);
	}

	inline void add(QWidget* widget)
	{
		auto [base_path, style_path] = sheetPathsFromClass(widget);
		m_stylees << Stylee{ widget, base_path, style_path };
	}

private:
	struct Stylee {
		QWidget* widget;
		StdFsPath baseSheet;
		StdFsPath styleSheet;
		StdFsPath currentThemeSheet = StdFsPath();
		bool hasTheme = true;
	};

	QVector<Stylee> m_stylees;

	inline QPair<StdFsPath, StdFsPath> sheetPathsFromClass(QWidget* widget)
	{
		QString class_name = widget->metaObject()->className();
		auto root = Path::toStdFs(":/stylist");
		auto base_filename = class_name + "Base.qss";
		auto style_filename = class_name + ".qss";
		auto base_path = root / base_filename.toStdString();
		auto style_path = root / style_filename.toStdString();
		return qMakePair(base_path, style_path);
	}
};
