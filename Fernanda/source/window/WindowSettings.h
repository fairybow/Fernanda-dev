#pragma once

#include "../common/IniWriter.hpp"
#include "../common/Path.hpp"
#include "../menu/ActionGroup.hpp"
#include "Window.h"

#include <QAction>
#include <QDebug>
#include <QDialog>
//#include <QFont>
#include <QList>
#include <QVariant>

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
	void applyAll(Window* window);
	void applyAll(QList<Window*>& windows);
	void applySetting(Window* window, Type type); // Clean this up somehow
	void applySetting(QList<Window*>& windows, Type type); // Clean this up somehow

private:
	struct ActionData {
		Type type = Type();
		QVariant fallback;
	};

	IniWriter* m_iniWriter;
	QDialog* m_dialog = nullptr;
	ActionGroup* m_meterGroup = new ActionGroup(this, "Meter");

	void applySettingByAction(Window* window, QAction* action); // Clean this up somehow
	void applyCheckedSettingByType(Window* window, Type type); // Clean this up somehow

	Type typeOf(QAction* action);
	ActionData dataOf(QAction* action);
	QAction* actionWith(Type type);

	void setupDialog(QDialog* dialog);
	void setupMeterGroup();
	QList<ActionGroup*> actionGroups() const;
	ActionData dataOf(QAction* action) const;
	QString iniKeyName(QAction* action) const;
	void loadGroupValues(ActionGroup* group);
	//void saveGroupValues(ActionGroup* group);

	template<typename T>
	QAction* newAction(ActionGroup* group, const QString& name, Type type, T fallback)
	{
		auto action = new QAction(name, group);
		auto action_data = ActionData(type, QVariant::fromValue(fallback));
		action->setData(QVariant::fromValue(action_data));
		connect(action, &QAction::toggled, this, &WindowSettings::onQActionToggled);
		// ^ Will this work if not all are checkable?

		return action;
	}

private slots:
	void onQActionToggled(bool checked);
	void onQDialogFinished();

signals:
	void settingChanged(Type type);
	//void currentFontChanged(const QFont& font);
};
