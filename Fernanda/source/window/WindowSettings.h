#pragma once

#include "../common/IniWriter.hpp"
#include "../common/Path.hpp"
#include "../menu/SettingsMaps.hpp"
#include "Window.h"

#include <QDebug>
#include <QDialog>
#include <QFont>
#include <QList>
#include <QVariant>

class WindowSettings : public QObject
{
	Q_OBJECT

public:
	WindowSettings(const Path& config, QObject* parent = nullptr);
	~WindowSettings();

	enum class Type {
		Font, // QFont
		EditorTheme, // path (one of a group)
		WindowTheme, // path (one of a group)
		MeterLinePos, // bool
		MeterColPos, // bool
		MeterLineCount, // bool
		MeterWordCount, // bool
		MeterCharCount, // bool
		End
	};

	void openDialog();
	void applySetting(Window* window, Type type);
	void applySetting(QList<Window*>& windows, Type type);
	void applyAll(Window* window);
	void applyAll(QList<Window*>& windows);

private:
	IniWriter* m_iniWriter;
	QDialog* m_dialog = nullptr;
	QFont m_font = QFont();
	ActionGroupSet* m_themes = new ActionGroupSet(this, "Themes");
	ActionSet* m_meterActionSet = new ActionSet(this, "Meter");

	QList<ActionSet*> actionSets() const;
	QList<ActionGroupSet*> groupSets() const;
	void setupMeterActionSet();
	void loadActionSetValues(ActionSet* actionSet);
	void saveActionSetValues(ActionSet* actionSet);
	void loadGroupSetValues(ActionGroupSet* groupSet);
	void saveGroupSetValues(ActionGroupSet* groupSet);
	QString iniKeyName(QString text);
	QVariant toVariant(Type type);
	void setupDialog(QDialog* dialog);
	Type typeData(QAction* action);
	QVariant currentValue(Type type);

private slots:
	void onQActionToggled(bool);
	void onQDialogFinished();

signals:
	void settingChanged(Type type);
	//void currentFontChanged(const QFont& font);
};
