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

public:
	inline Stylist(QVector<QWidget*> widgets, QObject* parent = nullptr, const QString& baseSuffix = "Base", const QString& extension = ".qss")
		: QObject(parent), m_baseSuffix(baseSuffix), m_extension(extension)
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

	inline std::pair<StdFsPath, StdFsPath> sheetPathsFromClass(QWidget* widget)
	{
		QString class_name = widget->metaObject()->className();
		auto root = Path::toStdFs(":/stylist");
		auto base_filename = class_name + m_baseSuffix + m_extension;
		auto style_filename = class_name + m_extension;
		auto base_path = root / base_filename.toStdString();
		auto style_path = root / style_filename.toStdString();
		return { base_path, style_path };
	}

	inline void apply(std::function<void(Stylee&)> function)
	{
		for (auto i = 0; i < m_stylees.count(); ++i)
			function(m_stylees[i]);
	}

	inline void styleAll(Mode mode = Mode::Style)
	{
		apply([&](Stylee& stylee) {
			(mode == Mode::Style)
				? stylee.widget->setStyleSheet(buildFullStyleSheet(stylee))
				: stylee.widget->setStyleSheet(nullptr);
			});
	}

	inline QString buildFullStyleSheet(const Stylee& stylee)
	{
		auto full_style_sheet = Io::readFile(stylee.baseSheet);
		if (stylee.hasTheme && stylee.currentThemeSheet != StdFsPath()) {
			auto theme_sheet = Io::readFile(stylee.currentThemeSheet);
			full_style_sheet += "\n" + buildThemedStyleSheet(Io::readFile(stylee.styleSheet), theme_sheet);
		}
		return full_style_sheet;
	}

	inline const QString buildThemedStyleSheet(QString styleSheet, const QString& themeSheet)
	{
		static const auto theme_sheet_line = QRegularExpression("(@.*\\n?)");
		auto matches = theme_sheet_line.globalMatch(themeSheet);
		while (matches.hasNext()) {
			auto match = matches.next();
			if (!match.hasMatch()) continue;
			auto [variable, value] = variableAndValue(match);
			styleSheet.replace(QRegularExpression(variable), value);
		}
		return styleSheet;
	}

	inline std::pair<QString, QString> variableAndValue(const QRegularExpressionMatch& themeSheetMatch)
	{
		static const auto theme_sheet_value = QRegularExpression("(\\s=.*;)");
		static const auto theme_sheet_variable = QRegularExpression("(@.*=\\s)");
		QString variable = themeSheetMatch.captured(0).replace(theme_sheet_value, nullptr);
		QString value = themeSheetMatch.captured(0).replace(theme_sheet_variable, nullptr);
		return { variable, value };
	}
};
