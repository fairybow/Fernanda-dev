#pragma once

#include "common/Path.hpp"
#include "documents/Document.h"
#include "editor/Editor.h"
#include "meter/Meter.h"
#include "page-area/PageArea.h"
#include "tree-view/TreeView.h"

#include <QCloseEvent>
#include <QDebug>
#include <QList>
#include <QMainWindow>
#include <QSplitter>

class Window : public QMainWindow
{
	Q_OBJECT

public:
	Window();
	~Window() { qDebug() << __FUNCTION__; }

	enum class SwitchIfFound {
		No,
		Yes
	};

	bool find(const Path& path, SwitchIfFound switchIfFound = SwitchIfFound::Yes) const;
	void open(Document* document);
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

	QList<PageArea*> pageAreas() const;
	PageIndex pageIndexOf(Editor* editor) const;
	PageIndex pageIndexOf(const Path& path) const;
	Editor* editorAt(PageIndex pageIndex) const;
	Document* documentOf(Editor* editor) const;

	void setup();
	void addTreeView();
	void setupTreeView(TreeView* treeView);
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
