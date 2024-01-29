#include "../common/Connect.hpp"
#include "../common/Image.hpp"
#include "../common/Io.hpp"
#include "Window.h"

#include <QStatusBar>
#include <QTextBlock>

constexpr char LOGO_QRC_PATH[] = ":/test/Fernanda_64.png";

Window::Window()
	: QMainWindow()
{
	setup();
}

bool Window::find(const Path& path, SwitchIfFound switchIfFound) const
{
	auto page_index = pageIndexOf(path);
	if (!page_index.isValid()) return false;

	if (switchIfFound == SwitchIfFound::Yes) {
		m_currentPageArea = page_index.area;
		m_currentPageArea->setCurrentIndex(page_index.i);
		m_currentPageArea->currentWidget()->setFocus();
	}

	return true;
}

void Window::open(Document* document)
{
	auto editor = newEditor(document);

	auto title = document->title();
	auto index = m_currentPageArea->add(editor, title);

	m_currentPageArea->setCurrentIndex(index);
	m_currentPageArea->currentWidget()->setFocus(); // Any reason not to just use `editor`?
}

void Window::open(const Path& path)
{
	auto document = newDocument(path);

	open(document);
}

void Window::closeEvent(QCloseEvent* event)
{
	emit closing();

	QMainWindow::closeEvent(event);
}

QList<PageArea*> Window::pageAreas() const
{
	return m_splitter->findChildren<PageArea*>(Qt::FindDirectChildrenOnly);
}

QList<Editor*> Window::editors() const
{
	return m_editors;
}

Window::PageIndex Window::pageIndexOf(Editor* editor) const
{
	for (auto& page_area : pageAreas())
		for (auto i = 0; i < page_area->count(); ++i) {
			auto page_index = PageIndex(page_area, i);

			if (editor != editorAt(page_index)) continue;

			return page_index;
		}

	return PageIndex();
}

Window::PageIndex Window::pageIndexOf(const Path& path) const
{
	for (auto& page_area : pageAreas())
		for (auto i = 0; i < page_area->count(); ++i) {
			auto page_index = PageIndex(page_area, i);

			auto editor = editorAt(page_index);
			auto document = documentOf(editor);

			if (!document || document->path() != path) continue;

			return page_index;
		}

	return PageIndex();
}

Editor* Window::editorAt(PageIndex pageIndex) const
{
	return qobject_cast<Editor*>(pageIndex.area->widgetAt(pageIndex.i));
}

Document* Window::documentOf(Editor* editor) const
{
	return qobject_cast<Document*>(editor->document());
}

void Window::setup()
{
	setCentralWidget(m_splitter);
	setupTreeView();

	addPageArea();

	// Temporary
	auto status_bar = new QStatusBar;
	setStatusBar(status_bar);
	status_bar->addWidget(m_meter);
	status_bar->addPermanentWidget(m_treeViewToggle);
	// ^ Instead, if m_treeView present, show button on TreeView, else show on Window
}

void Window::setupTreeView()
{
	m_dockWidget->setWidget(m_treeView);
	addDockWidget(Qt::LeftDockWidgetArea, m_dockWidget);

	connect(m_treeViewToggle, &UiButton::clicked, this, [=] {
		m_dockWidget->toggleViewAction()->trigger();
		});

	connect(m_treeView, &TreeView::fileClicked, this, [&](const Path& path) {
		find(path);
		});
	connect(m_treeView, &TreeView::fileDoubleClicked, this, [&](const Path& path) {
		emit treeViewFileDoubleClicked(path);
		});
}

void Window::addPageArea()
{
	auto page_area = new PageArea;

	m_splitter->addWidget(page_area);
	auto index = m_splitter->indexOf(page_area);
	m_splitter->setCollapsible(index, false);

	setupPageArea(page_area);

	m_currentPageArea = page_area;
}

void Window::setupPageArea(PageArea* pageArea)
{
	pageArea->setHasScrolls(true);
	pageArea->setTabsElideMode(Qt::ElideRight);
	pageArea->setTabsMovable(true);
	pageArea->setDrawBarBase(true);
	pageArea->setBarExpanding(true); // Need a minimum width to work as false

	auto pixmap = Image::greyscale(LOGO_QRC_PATH);
	pageArea->setUnderlayPixmap(pixmap);

	connect(pageArea, &PageArea::addRequested, this, &Window::onPageAreaAddRequested);
	connect(pageArea, &PageArea::closeRequested, this, &Window::onPageAreaCloseRequested);
	connect(pageArea, &PageArea::currentChanged, this, &Window::onPageAreaCurrentChanged);
}

Document* Window::newDocument(const Path& path)
{
	auto document = new Document(this);

	if (path) {
		// Check first for autosaved text?
		auto text = Io::read(path);
		document->setText(text);
		document->setTitle(path.qStringName());
		document->setPath(path);
	}

	document->setModified(false); // Not if from autosaved?

	return document;
}

Editor* Window::newEditor(Document* document)
{
	auto editor = new Editor;
	m_editors << editor;

	editor->setDocument(document);
	editor->setFont(m_editorsFont);
	editor->setIsTypewriter(m_editorsIsTypewriter);

	connect(editor, &Editor::textChanged, this, &Window::onEditorTextChanged);
	connect(editor, &Editor::modificationChanged, this, &Window::onEditorModificationChanged);

	return editor;
}

Editor* Window::removeCurrentPageAreaEditor(int index)
{
	auto editor = m_currentPageArea->remove(index);

	return qobject_cast<Editor*>(editor);
}

void Window::onPageAreaAddRequested()
{
	auto page_area = sender_cast(PageArea);
	if (!page_area) return;

	m_currentPageArea = page_area;

	open();
}

void Window::onPageAreaCloseRequested(int index)
{
	auto page_area = sender_cast(PageArea);
	if (!page_area) return;

	m_currentPageArea = page_area;

	auto editor = removeCurrentPageAreaEditor(index);
	auto document = documentOf(editor);

	if (editor) {
		m_editors.removeAll(editor);

		delete editor;
	}

	// What to do with Document?

	auto current_editor = m_currentPageArea->currentWidget();

	if (current_editor)
		current_editor->setFocus();
}

void Window::onPageAreaCurrentChanged(int index)
{
	auto page_area = sender_cast(PageArea);
	if (!page_area) return;

	m_currentPageArea = page_area;

	if (index < 0) {
		m_meter->reset();

		return;
	}

	auto editor = editorAt({ m_currentPageArea, index });
	if (!editor) return;
	
	m_meter->setCurrentEditor(editor);
}

void Window::onEditorModificationChanged(bool changed)
{
	auto editor = sender_cast(Editor);
	if (!editor) return;

	auto page_index = pageIndexOf(editor);
	if (!page_index.isValid()) return;

	page_index.area->setTabFlagged(page_index.i, changed);
}

void Window::onEditorTextChanged()
{
	auto editor = sender_cast(Editor);
	if (!editor) return;

	auto document = documentOf(editor);
	if (document->hasTitle()) return;

	auto page_index = pageIndexOf(editor);
	if (!page_index.isValid()) return;

	auto block = document->firstBlock().text();
	page_index.area->setPageTabText(page_index.i, block.left(30));
}
