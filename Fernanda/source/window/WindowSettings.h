#pragma once

#include "../common/IniWriter.hpp"
#include "../common/Path.hpp"
#include "Window.h"

#include <QDebug>
#include <QDialog>
#include <QEvent>
#include <QFont>
#include <QList>
#include <QMap>
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
	void loadEditorSettings();
	void loadMeterSettings();
	void loadWindowSettings();
	void saveAll();
	void saveEditorSettings();
	void saveMeterSettings();
	void saveWindowSettings();
	QString iniName(QString text);
	void setupDialog(QDialog* dialog);

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
