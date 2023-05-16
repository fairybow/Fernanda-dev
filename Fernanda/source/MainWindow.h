#pragma once

#include "common/Emoji.hpp"
#include "common/Layout.hpp"
#include "common/Widget.hpp"
#include "document/Document.h"
#include "editor/Editor.h"
#include "menu/MenuBar.h"
#include "preview/Preview.hpp"
#include "tools/AlwaysOnTop.hpp"
#include "tools/StayAwake.hpp"
#include "tools/PomodoroTimer.hpp"
#include "user/User.hpp"
#include "Indicator.hpp"
#include "IniKeys.hpp"
#include "Meter.h"
#include "StatusBar.hpp"
#include "Stylist.h"
#include "TabBar.h"

#include <QCloseEvent>
#include <QMainWindow>
#include <QShowEvent>
#include <QString>

#include <filesystem>
#include <functional>

class MainWindow : public Widget<QMainWindow>
{
	using StdFsPath = std::filesystem::path;

	Q_OBJECT

public:
	MainWindow(const char* name, bool isDev = false, StdFsPath file = StdFsPath(), QWidget* parent = nullptr);

protected:
	virtual void closeEvent(QCloseEvent* event) override;
	virtual void showEvent(QShowEvent* event) override;

private:
	User* m_user = new User(QCoreApplication::applicationName(), this);
	Document* m_document = new Document(m_user->temp(), m_user->backup(), this);
	//Project* m_project = new Project(this);
	MenuBar* m_menuBar = new MenuBar("MenuBar", m_user->data(), m_user->documents(), m_isDev);
	StatusBar* m_statusBar = new StatusBar("StatusBar");
	Indicator* m_indicator = new Indicator("Indicator");
	TabBar* m_tabBar = new TabBar("TabBar");
	Editor* m_editor = new Editor("Editor", QFont("mononoki", 12));
	//Preview* m_preview = new Preview("Preview");
	Meter* m_meter = new Meter("Meter");
	PomodoroTimer* m_pomodoroTimer = new PomodoroTimer(Emoji::TOMATO, this);
	StayAwake* m_stayAwake = new StayAwake(Emoji::TEACUP);
	AlwaysOnTop* m_alwaysOnTop = new AlwaysOnTop(Emoji::PUSHPIN, this);
	Stylist* m_stylist = new Stylist({ this, m_editor }, this);

	const bool m_isDev;
	bool m_isInitialized = false;

	void setupWidgets();
	void connections();
	void documentConnections();
	void tabBarConnections();
	void editorConnections();
	void meterConnections();
	//void previewConnections();
	void menuBarConnections();
	void menuBarStyleConfigConnections();
	void menuBarEditorConfigConnections();
	void menuBarMeterConfigConnections();
	void menuBarToolConfigConnections();
	void menuBarMiscConfigConnections();
	void loadConfigs();
	void loadEditorConfigs();
	//void loadPreviewConfigs();
	void loadMenuBarStyleConfigs();
	void loadMenuBarEditorConfigs();
	void loadMenuBarMeterConfigs();
	void loadMenuBarToolConfigs();
	void loadMenuBarMiscConfigs();
	void closeEventConfigs(Qt::WindowStates priorState);
	void menuBarOpenFile(StdFsPath path, bool writeNew = false);

	void menuBarOpenNewFile(StdFsPath path) { menuBarOpenFile(path, true); };

	template<typename T>
	void saveConfigPassthrough(T value, const QString& valueKey, QObject* associatedObject, std::function<void()> configurableAction = nullptr)
	{
		if (configurableAction)
			configurableAction();
		m_user->save(value, valueKey, associatedObject);
	}

	template<typename T>
	void loadConfigPassthrough(const QString& valueKey, QObject* associatedObject, std::function<void(T)> configurableAction, T fallbackValue = T())
	{
		auto value = m_user->load<T>(valueKey, associatedObject, fallbackValue);
		configurableAction(value);
	}

	template<typename T>
	T loadConfig(const QString& valueKey, QObject* associatedObject, T fallbackValue = T())
	{
		return m_user->load<T>(valueKey, associatedObject, fallbackValue);
	}

private slots:
	void openTab(int index);
	void newTab();
};
