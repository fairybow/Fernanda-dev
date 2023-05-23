#pragma once

#include "../common/Layout.hpp"
#include "../common/Path.hpp"
#include "../common/Widget.hpp"
#include "AddTab.hpp"
#include "CloseTab.hpp"
#include "ScrollTabs.hpp"
#include "TrueTabBar.hpp"

#include <QString>
#include <QUuid>
#include <QVariantMap>
#include <QVector>

#include <filesystem>

class TabBar : public Widget<>
{
	using StdFsPath = std::filesystem::path;

	Q_OBJECT

public:
	TabBar(const char* name, int minTabSize = 25, int maxTabSize = 100, QWidget* parent = nullptr);

	int serve(QUuid id, StdFsPath pathForTitle = StdFsPath(), bool switchTo = true);
	QUuid tabId(int index);
	const QString title(int index);
	int index(QUuid id);
	bool isUntitled();
	void setUntitledDisplay(const QString& text, int charLimit = 30);

signals:
	void currentChanged(int index);
	void askNew();

public slots:
	void updateEditedState(QUuid id, bool edited);

private:
	static constexpr char DATA_ID[] = "tab_id";
	static constexpr char DATA_TITLE[] = "tab_title";

	TrueTabBar* m_trueTabBar;
	AddTab* m_add = new AddTab;
	ScrollTabs* m_scrollLeft = new ScrollTabs(m_trueTabBar, ScrollTabs::Side::Left);
	ScrollTabs* m_scrollRight = new ScrollTabs(m_trueTabBar, ScrollTabs::Side::Right);

	void setupWidgets();
	void connections();
	bool isFull();
	void adjustControls();
	int create(QUuid id, StdFsPath titlePath = StdFsPath());
	void setButton(int index, QUuid id);
	void setData(int index, QUuid id, QString title = QString());
};
