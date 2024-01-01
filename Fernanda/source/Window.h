#pragma once

#include "common/Path.hpp"
#include "documents/Document.h"
#include "meter/Meter.h"
#include "page-area/PageArea.h"
#include "tree-view/TreeView.h"

#include <QCloseEvent>
#include <QDebug>
#include <QMainWindow>
#include <QSplitter>

class Window : public QMainWindow
{
	Q_OBJECT

public:
	enum class SwitchIfFound {
		No,
		Yes
	};

	Window();
	~Window() { qDebug() << __FUNCTION__; }

	bool find(const Path& path, SwitchIfFound switchIfFound = SwitchIfFound::Yes) const;
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
	PageIndex pageIndexOf(Document* document) const;
	PageIndex pageIndexOf(const Path& path) const;

	void setup();
	void addTreeView();
	void setupTreeView(TreeView* treeView);
	void addPageArea();
	void setupPageArea(PageArea* pageArea);
	Document* newDocument(const Path& path = Path());

private slots:
	void onPageAreaAddRequested();
	void onPageAreaCloseRequested(int index);
	void onPageAreaCurrentChanged(int index);
	void onDocumentModificationChanged(bool changed);
	void onEditorTextChanged();

signals:
	void treeViewFileDoubleClicked(const Path& path);
	void closing();
};
