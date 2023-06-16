#pragma once

#include "../common/Layout.hpp"
#include "../common/Path.hpp"
#include "../common/Widget.hpp"
#include "AddTab.hpp"
#include "CloseTab.hpp"
#include "ScrollTabs.hpp"
#include "TrueTabBar.hpp"

#include <QApplication>
#include <QString>
#include <QUuid>
#include <QVariantMap>
#include <QVector>
#include <QWheelEvent>

#include <filesystem>

class TabBar : public Widget<>
{
	Q_OBJECT

public:
	using StdFsPath = std::filesystem::path;

	TabBar(const char* name, int minTabSize = 25, int maxTabSize = 100, QWidget* parent = nullptr);

	int serve(const QUuid& id, StdFsPath pathForTitle = StdFsPath(), bool switchTo = true); // don't take Document, but change path to QString title
	bool isUntitled();
	void setUntitledDisplay(const QString& text, int charLimit = 30);
	void close(const QUuid& id);
	bool isFull();
	bool isEmpty();

signals:
	void currentChanged(const QUuid& id);
	void askAdd();
	void askClearForClose(const QUuid& id);

public slots:
	void updateEditedState(const QUuid& id, bool edited);
	void updateTitle(const QUuid& id, const QString& title);

protected:
	virtual void wheelEvent(QWheelEvent* event) override;

private:
	static constexpr char DATA_ID[] = "tab_id";
	static constexpr char DATA_TITLE[] = "tab_title";

	TrueTabBar* m_trueTabBar;
	AddTab* m_add = new AddTab;
	ScrollTabs* m_scrollLeft = new ScrollTabs(m_trueTabBar, ScrollTabs::Side::Left);
	ScrollTabs* m_scrollRight = new ScrollTabs(m_trueTabBar, ScrollTabs::Side::Right);

	void setupWidgets();
	void connections();
	QUuid idByIndex(int index);
	int indexById(const QUuid& id);
	const QString title(int index);
	int create(const QUuid& id, StdFsPath titlePath = StdFsPath());
	void setButton(int index, const QUuid& id);
	void setData(int index, const QUuid& id, QString title = QString());
	CloseTab* closeButton(const QUuid& id);

private slots:
	void adjustControls();
};
