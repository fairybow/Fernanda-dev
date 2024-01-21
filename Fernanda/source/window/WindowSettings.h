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
	struct Action {
		QVariant variant;
		std::function<void(Window*)> action;
	};

	IniWriter* m_iniWriter;
	QDialog* m_dialog = nullptr;
	QList<Window*> m_windows;
	QMap<QString, QMap<QString, Action>> m_settings;

	void loadAll();
	void loadEditorSettings();
	void loadMeterSettings();
	void loadWindowSettings();
	void saveAll();
	void saveEditorSettings();
	void saveMeterSettings();
	void saveWindowSettings();
	void applySetting(const QString& prefix, const QString& key);
	void applyAll(Window* window);
	QString iniName(QString text);
	void setupDialog(QDialog* dialog);

	template <typename T>
	void saveSetting(const QString& prefix, const QString& key, T value)
	{
		m_settings[prefix][key].variant = QVariant::fromValue<T>(value);
	}

	template <typename T>
	void saveAndApplySetting(const QString& prefix, const QString& key, T value)
	{
		saveSetting<T>(prefix, key, value);

		applySetting(prefix, key);
	}

private slots:
	void onDialogFinished();
};
