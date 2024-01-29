#pragma once

#include "../common/IniWriter.hpp"
#include "../common/Path.hpp"
#include "Window.h"

#include <QCheckBox>
#include <QDebug>
#include <QDialog>
#include <QEvent>
#include <QFont>
#include <QGroupBox>
#include <QList>
#include <QMap>
#include <QMargins>
#include <QObject>
#include <QString>
#include <QVariant>

#include <functional>

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
	enum class AppendPrefix {
		No,
		Yes
	};

	struct Setting {
		QVariant variant;
		std::function<void(Window*)> action;

		template <typename T>
		T value()
		{
			return variant.value<T>();
		}
	};

	IniWriter* m_iniWriter;
	QDialog* m_dialog = nullptr;
	QList<Window*> m_windows;
	QMap<QString, QMap<QString, Setting>> m_settings;

	void loadAll();
	void loadDataSettings();
	void loadEditorSettings();
	void loadMeterSettings();
	void loadWindowSettings();
	void saveAll();
	void saveSetting(const QString& prefix, const QString& key, AppendPrefix usePrefix = AppendPrefix::Yes);
	void saveSettings(const QString& prefix, QStringList keys);
	QString iniName(QString text);
	void setupDialog(QDialog* dialog);
	QGroupBox* newGroupBox(QDialog* dialog, QLayout* layout, const QMargins& margins, int spacing);
	QGroupBox* newDataBox(QDialog* dialog, const QMargins& margins, int spacing);
	QGroupBox* newEditorBox(QDialog* dialog, const QMargins& margins, int spacing);
	QGroupBox* newFontBox(QDialog* dialog, const QMargins& margins, int spacing);
	QGroupBox* newMeterBox(QDialog* dialog, const QMargins& margins, int spacing);
	QCheckBox* newCheckBox(const QString& prefix, const QString& key, QWidget* parent);
	void moveXYIfTaken(Window* window);
	void syncUp(const QString& prefix, const QString& key);
	void applyAll(Window* window);
	QVariant variantAt(const QString& prefix, const QString& key);

	template <typename T>
	void passiveApply(const QString& prefix, const QString& key, T value)
	{
		m_settings[prefix][key].variant = QVariant::fromValue<T>(value);
	}

	template <typename T>
	void activeApply(const QString& prefix, const QString& key, T value)
	{
		passiveApply<T>(prefix, key, value);

		syncUp(prefix, key);
	}

	template <typename T>
	T valueAt(const QString& prefix, const QString& key)
	{
		return m_settings[prefix][key].value<T>();
	}

private slots:
	void onDialogFinished();
};
