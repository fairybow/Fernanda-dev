#pragma once

#include "common/Layout.hpp"
#include "common/Path.hpp"
#include "common/Widget.hpp"
#include "document/Document.h"
#include "editor/Editor.h"
#include "menu-bar/MenuBar.h"
#include "preview/Preview.hpp"
#include "tab-bar/TabBar.h"
#include "tools/AlwaysOnTop.hpp"
#include "tools/StayAwake.hpp"
#include "tools/PomodoroTimer.h"
#include "user/User.hpp"
#include "Indicator.hpp"
#include "IniKeys.hpp"
#include "Meter.h"
#include "StatusBar.hpp"
#include "Stylist.h"

#include <QCloseEvent>
#include <QDirIterator>
#include <QMainWindow>
#include <QMessageBox>
#include <QShowEvent>
#include <QString>
#include <QUuid>

#include <filesystem>
#include <functional>

class MainWindow : public Widget<QMainWindow>
{
	Q_OBJECT

public:
	using StdFsPath = std::filesystem::path;
	using PromptResult = QMessageBox::StandardButton;

	MainWindow(const char* name, bool isDev = false, StdFsPath file = StdFsPath(), QWidget* parent = nullptr);

	StdFsPath userData() const { return m_user->data(); }

protected:
	virtual void closeEvent(QCloseEvent* event) override;
	virtual void showEvent(QShowEvent* event) override;

private:
	const bool m_isDev;
	bool m_isInitialized = false;

	User* m_user = new User(QCoreApplication::applicationName(), this);
	Document* m_document = new Document(m_user->temp(), m_user->backup(), this, 3/* <-- test */);
	//Project* m_project = new Project(this);
	MenuBar* m_menuBar = new MenuBar("MenuBar", m_user->data(), m_user->documents(), m_isDev);
	StatusBar* m_statusBar = new StatusBar("StatusBar");
	Indicator* m_indicator = new Indicator("Indicator");
	TabBar* m_tabBar = new TabBar("TabBar", 100, 200);
	Editor* m_editor = new Editor("Editor", QFont("mononoki", 14));
	//Preview* m_preview = new Preview("Preview");
	Meter* m_meter = new Meter("Meter");
	PomodoroTimer* m_pomodoroTimer = new PomodoroTimer(this);
	StayAwake* m_stayAwake = new StayAwake;
	AlwaysOnTop* m_alwaysOnTop = new AlwaysOnTop(this);
	Stylist* m_stylist = new Stylist({ this, m_editor }, this);

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
	void menuBarDevConnections();
	void loadConfigs();
	void loadEditorConfigs();
	//void loadPreviewConfigs();
	void loadMenuBarStyleConfigs();
	void loadMenuBarEditorConfigs();
	void loadMenuBarMeterConfigs();
	void loadMenuBarToolConfigs();
	void loadMenuBarMiscConfigs();
	void closeEventConfigs(Qt::WindowStates priorState);
	void setUserFont(const QFont& font);
	PromptResult singleSavePrompt();

	void openFileTab(StdFsPath path, bool writeNew = false);
	void openNewFileTab(StdFsPath path) { openFileTab(path, true); };
	void openNewTab() { onAddTabClick(); };

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
	void onTabClick(QUuid id);
	void onAddTabClick();
	void onCloseTabClick(QUuid id);
	bool onSaveFile();
};
