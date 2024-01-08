#pragma once

#include "../common/IniWriter.hpp"
#include "../common/Path.hpp"
#include "Window.h"

#include <QAction>
#include <QCheckBox>
#include <QDebug>
#include <QDialog>
#include <QGroupBox>
#include <QList>
#include <QMap>
#include <QString>

class WindowSettings : public QObject
{
	Q_OBJECT

public:
	WindowSettings(const Path& config, QObject* parent = nullptr);
	~WindowSettings() { qDebug() << __FUNCTION__; }

	enum class Type {
		MeterLinePos,
		MeterColPos,
		MeterLineCount,
		MeterWordCount,
		MeterCharCount
	};

	void openDialog();
	void applySetting(Window* window, bool checked, Type type);
	void applySetting(QList<Window*> windows, bool checked, Type type);

private:
	static constexpr char PREFIX_METER[] = "Meter";

	IniWriter* m_iniWriter;
	QDialog* m_dialog = nullptr;
	QMap<QString, QAction*> m_namesToActions;

	enum class Prefix {
		No,
		Yes
	};

	void setup();
	void setupMeterActions();
	QAction* makeAction(Type type);
	void setupDialog();
	QGroupBox* meterBox();
	QCheckBox* makeCheckBox(Type type);
	constexpr const char* name(Type type, Prefix prefix = Prefix::No);
	QString iniName(Type type, Prefix prefix = Prefix::No);

private slots:
	void onQActionToggled(bool checked);
	void onQDialogFinished();

signals:
	void currentFontChanged(const QFont& font);
	void settingChecked(bool checked, Type type);
};
