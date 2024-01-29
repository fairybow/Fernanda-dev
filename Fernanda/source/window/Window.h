#pragma once

#include "../common/Path.hpp"
#include "../objects/Document.h" // Removed from structure (above)--so maybe a sign we should do without it
#include "widgets/Editor.h"
#include "widgets/Meter.h"
#include "widgets/page-area/PageArea.h"
#include "widgets/TreeView.h"

#include <QCloseEvent>
#include <QDebug>
#include <QDockWidget>
#include <QFont>
#include <QList>
#include <QMainWindow>
#include <QSplitter>
//
#include "../common/UiButton.hpp"

class WindowSettings;

class Window : public QMainWindow
{
	friend class WindowSettings;

	Q_OBJECT

public:
	Window();
	~Window() { qDebug() << __FUNCTION__; }

	enum class SwitchIfFound {
		No,
		Yes
	};

	bool find(const Path& path, SwitchIfFound switchIfFound = SwitchIfFound::Yes) const;
	// ^ May move to Fernanda eventually, if this can operate without Document->path()
	void open(Document* document); // Can possibly switch to QTextDocument
	void open(const Path& path = Path());

protected:
	void closeEvent(QCloseEvent* event) override;

private:
	struct PageIndex {
		PageArea* area;
		int i;

		PageIndex(PageArea* area = nullptr, int i = -1)
			: area(area), i(i)
		{}

		bool isValid() const
		{
			return area != nullptr && i > -1;
		}
	};

	QSplitter* m_splitter = new QSplitter(this);
	mutable PageArea* m_currentPageArea = nullptr;
	Meter* m_meter = new Meter;
	QList<Editor*> m_editors;
	QFont m_editorsFont = QFont();
	bool m_editorsIsTypewriter = false;
	QDockWidget* m_dockWidget = new QDockWidget;
	TreeView* m_treeView = new TreeView;
	UiButton* m_treeViewToggle = new UiButton(UiButton::Ui::MenuOpen, this);

	QList<PageArea*> pageAreas() const;
	QList<Editor*> editors() const;
	PageIndex pageIndexOf(Editor* editor) const;
	PageIndex pageIndexOf(const Path& path) const;
	Editor* editorAt(PageIndex pageIndex) const;
	Document* documentOf(Editor* editor) const;

	void setup();
	void setupTreeView();
	void addPageArea();
	void setupPageArea(PageArea* pageArea);
	Document* newDocument(const Path& path = Path());
	Editor* newEditor(Document* document);
	Editor* removeCurrentPageAreaEditor(int index);


private slots:
	void onPageAreaAddRequested();
	void onPageAreaCloseRequested(int index);
	void onPageAreaCurrentChanged(int index);
	void onEditorModificationChanged(bool changed);
	void onEditorTextChanged();

signals:
	void treeViewFileDoubleClicked(const Path& path);
	void closing();
};
