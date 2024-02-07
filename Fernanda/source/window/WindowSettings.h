#pragma once

#include "../common/IniWriter.hpp"
#include "../common/Path.hpp"
#include "settings-objects/Setting.hpp"
#include "Window.h"

#include <QCheckBox>
#include <QDialog>
#include <QEvent>
#include <QGroupBox>
#include <QHash>
#include <QList>
#include <QMargins>
#include <QString>
#include <QVariant>

class WindowSettings : public QObject
{
	Q_OBJECT

public:
	WindowSettings(const Path& config, QObject* parent = nullptr);
	~WindowSettings();

	void yoke(Window* window);
	void detach(Window* window);
	void openDialog();
	bool eventFilter(QObject* watched, QEvent* event) override;

private:
	struct SettingInfo {
		// How to handle lambda/std::function?
		QString key;
		QVariant fallback;
		void(WindowSettings::* action)(Window*);

		SettingInfo(const QString& key, const QVariant& fallback, void(WindowSettings::* action)(Window*))
			: key(key), fallback(fallback), action(action)
		{}

		SettingInfo(const QString& key, void(WindowSettings::* action)(Window*))
			: SettingInfo(key, QVariant(), action)
		{}
	};

	IniWriter* m_iniWriter;
	QDialog* m_dialog = nullptr;
	QList<Window*> m_windows;
	QHash<QString, QHash<QString, Setting<Window*>>> m_settings;

	QString iniName(QString text) const;
	void loadAllIniValues();
	void loadIniValues(const QString& prefix, QList<SettingInfo> infos);
	void saveAllIniValues();
	void saveIniValues(const QString& prefix, QStringList keys);

	void applyToAll(const QString& prefix, const QString& key);
	void applyAllTo(Window* window);
	QVariant variantAt(const QString& prefix, const QString& key) const;

	void setDataProjectsPath(Window* window);
	void setEditorCenterOnScroll(Window* window);
	void setEditorFont(Window* window);
	void setEditorTypewriter(Window* window);
	void setEditorWrap(Window* window);
	void setMeterLinePosition(Window* window);
	void setMeterColumnPosition(Window* window);
	void setMeterLineCount(Window* window);
	void setMeterWordCount(Window* window);
	void setMeterCharCount(Window* window);
	void setMeterPositionLabels(Window* window);
	void setMeterCountLabels(Window* window);
	void setMeterShortLabels(Window* window);
	void setWindowDockPosition(Window* window);
	void setWindowDockVisibility(Window* window);
	void setWindowGeometry(Window* window);

	void setupDialog(QDialog* dialog);
	QGroupBox* newGroupBox(QDialog* dialog, QLayout* layout, const QMargins& margins, int spacing);
	QGroupBox* newDataBox(QDialog* dialog, const QMargins& margins, int spacing);
	QGroupBox* newEditorBox(QDialog* dialog, const QMargins& margins, int spacing);
	QGroupBox* newFontBox(QDialog* dialog, const QMargins& margins, int spacing);
	QGroupBox* newMeterBox(QDialog* dialog, const QMargins& margins, int spacing);
	QCheckBox* newCheckBox(const QString& prefix, const QString& key, QWidget* parent);

	template <typename T>
	void passiveApply(const QString& prefix, const QString& key, const T& value)
	{
		m_settings[prefix][key].setValue<T>(value);
	}

	template <typename T>
	void activeApply(const QString& prefix, const QString& key, const T& value)
	{
		passiveApply<T>(prefix, key, value);

		applyToAll(prefix, key);
	}

	template <typename T>
	T valueAt(const QString& prefix, const QString& key) const
	{
		return m_settings[prefix][key].value<T>();
	}

private slots:
	void onDialogFinished();
};
