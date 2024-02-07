#pragma once

#include "../common/IniWriter.hpp"
#include "../common/Path.hpp"
#include "Window.h"

#include <QCheckBox>
#include <QDialog>
#include <QEvent>
#include <QGroupBox>
#include <QList>
#include <QMap>
#include <QMargins>
#include <QObject>
#include <QString>
#include <QVariant>

#include <functional>

class Setting
{
public:
	Setting() = default;

	template <typename T>
	Setting(const T& value, std::function<void(Window*)> action)
		: m_variant(QVariant::fromValue<T>(value)), m_action(action)
	{}

	template <typename T, typename TClass>
	Setting(const T& value, TClass* context, void (TClass::* action)(Window*))
		: m_variant(QVariant::fromValue<T>(value)), m_action(lambdaWrap<TClass>(context, action))
	{}

	QVariant variant() const
	{
		return m_variant;
	}

	void setVariant(const QVariant& variant)
	{
		m_variant = variant;
	}

	template <typename T>
	T value()
	{
		return m_variant.value<T>();
	}

	template <typename T>
	void setValue(const T& value)
	{
		m_variant = QVariant::fromValue<T>(value);
	}

	void action(Window* window)
	{
		m_action(window);
	}

private:
	QVariant m_variant;
	std::function<void(Window*)> m_action;
	//QList<QVariant> m_pool;

	template <typename TClass>
	auto lambdaWrap(TClass* context, void (TClass::* action)(Window*))
	{
		return [=](Window* window) { std::invoke(action, context, window); };
	}
};

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
	/*enum class AppendPrefix {
		No,
		Yes
	};*/

	/*struct Setting {
		QVariant variant;
		std::function<void(Window*)> action;
		QList<QVariant> pool;

		template <typename T>
		T value()
		{
			return variant.value<T>();
		}
	};*/

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
	void saveSetting(const QString& prefix, const QString& key);
	void saveSettings(const QString& prefix, QStringList keys);
	QVariant loadSetting(const QString& key, const QVariant& fallback = QVariant()) const;
	QString iniName(QString text) const;
	void setupDialog(QDialog* dialog);
	QGroupBox* newGroupBox(QDialog* dialog, QLayout* layout, const QMargins& margins, int spacing);
	QGroupBox* newDataBox(QDialog* dialog, const QMargins& margins, int spacing);
	QGroupBox* newEditorBox(QDialog* dialog, const QMargins& margins, int spacing);
	QGroupBox* newFontBox(QDialog* dialog, const QMargins& margins, int spacing);
	QGroupBox* newMeterBox(QDialog* dialog, const QMargins& margins, int spacing);
	QCheckBox* newCheckBox(const QString& prefix, const QString& key, QWidget* parent);
	void syncUp(const QString& prefix, const QString& key);
	void applyAll(Window* window);
	QVariant variantAt(const QString& prefix, const QString& key);

	void setDataProjectsPath(Window* window);
	void setEditorCenterOnScroll(Window* window);
	void setEditorFont(Window* window);
	void setEditorTypewriter(Window* window);
	void setMeterPositionLabels(Window* window);
	void setMeterCountLabels(Window* window);
	void setMeterShortLabels(Window* window);
	void setWindowDockPosition(Window* window);
	void setWindowGeometry(Window* window);

	template <typename T>
	void passiveApply(const QString& prefix, const QString& key, const T& value)
	{
		m_settings[prefix][key].setValue<T>(value);
	}

	template <typename T>
	void activeApply(const QString& prefix, const QString& key, const T& value)
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
