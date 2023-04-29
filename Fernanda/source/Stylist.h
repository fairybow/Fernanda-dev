#pragma once

#include "common/Io.hpp"
#include "common/Path.hpp"

#include <QRegularExpressionMatchIterator>
#include <QVector>
#include <QWidget>

#include <functional>
#include <utility>

class Stylist : public QObject
{
	using StdFsPath = std::filesystem::path;
	using PathPair = std::pair<StdFsPath, StdFsPath>;
	using QStringPair = std::pair<QString, QString>;

public:
	Stylist(QVector<QWidget*> widgets = {}, QObject* parent = nullptr, const QString& baseSuffix = "Base", const QString& extension = ".qss");

	void add(QWidget* widget);
	void style(QWidget* stylee, const StdFsPath& themeSheet);
	void style();
	void unStyle();
	void setThemeEnabled(QWidget* styleeWidget, bool hasTheme);

private:
	enum class Mode {
		Style,
		Unstyle
	};

	struct Stylee {
		QWidget* widget;
		StdFsPath baseSheet;
		StdFsPath styleSheet;
		StdFsPath currentThemeSheet = StdFsPath();
		bool hasTheme = true;
	};

	const QString& m_baseSuffix;
	const QString& m_extension;
	QVector<Stylee> m_stylees;

	PathPair sheetPathsFromClass(QWidget* widget);
	void apply(std::function<void(Stylee&)> function);
	void styleAll(Mode mode = Mode::Style);
	void setCurrentThemeSheet(QWidget* styleeWidget, const StdFsPath& themeSheetPath);
	QString buildFullStyleSheet(const Stylee& stylee);
	const QString buildThemedStyleSheet(QString styleSheet, const QString& themeSheet);
	QStringPair variableAndValue(const QRegularExpressionMatch& themeSheetMatch);
};
