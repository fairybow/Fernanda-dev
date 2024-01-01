#include "common/Connect.hpp"
#include "common/Image.hpp"
#include "common/Io.hpp"
#include "editor/Editor.h"
#include "Window.h"

#include <QDockWidget>
#include <QStatusBar>
#include <QTextBlock>
#include <QVariant>

constexpr char LOGO_QRC_PATH[] = ":/test/Fernanda_64.png";

Window::Window()
	: QMainWindow()
{
	setup();

	// Temporary
	setGeometry(0, 0, 600, 400); // Given from settings
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

void Window::open(const Path& path)
{
	auto document = newDocument(path);
	connect(document, &Document::modificationChanged, this, &Window::onDocumentModificationChanged);
	// ^ Use `Editor::modificationChanged` instead?

	auto editor = new Editor;
	editor->setDocument(document);
	connect(editor, &Editor::textChanged, this, &Window::onEditorTextChanged);

	auto title = document->title();
	auto index = m_currentPageArea->add(editor, title);

	auto data = QVariant::fromValue(document);
	m_currentPageArea->setData(index, data);

	m_currentPageArea->setCurrentIndex(index);
	m_currentPageArea->currentWidget()->setFocus();
}

void Window::closeEvent(QCloseEvent* event)
{
	emit closing();

	QMainWindow::closeEvent(event);
}

QList<PageArea*> Window::pageAreas() const
{
	return m_splitter->findChildren<PageArea*>();
}

Window::PageIndex Window::pageIndexOf(Document* document) const
{
	for (auto& page_area : pageAreas())
		for (auto i = 0; i < page_area->count(); ++i) {
			auto data = page_area->data(i);

			if (document != data.value<Document*>()) continue;

			return PageIndex(page_area, i);
		}

	return PageIndex();
}

Window::PageIndex Window::pageIndexOf(const Path& path) const
{
	for (auto& page_area : pageAreas())
		for (auto i = 0; i < page_area->count(); ++i) {
			auto data = page_area->data(i);
			auto document = data.value<Document*>();

			if (!document || document->path() != path) continue;

			return PageIndex(page_area, i);
		}

	return PageIndex();
}

void Window::setup()
{
	setCentralWidget(m_splitter);

	addTreeView();
	addPageArea();

	// Temporary
	auto status_bar = new QStatusBar;
	setStatusBar(status_bar);
	status_bar->addWidget(m_meter);
}

void Window::addTreeView()
{
	auto tree_view = new TreeView;

	auto dock_widget = new QDockWidget;
	dock_widget->setWidget(tree_view);
	addDockWidget(Qt::LeftDockWidgetArea, dock_widget);

	setupTreeView(tree_view);
}

void Window::setupTreeView(TreeView* treeView)
{
	connect(treeView, &TreeView::fileClicked, this, [&](const Path& path) {
		find(path);
		});
	connect(treeView, &TreeView::fileDoubleClicked, this, [&](const Path& path) {
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

	auto editor = m_currentPageArea->remove(index);

	// What to do with Document?

	if (editor)
		delete editor;
}

void Window::onPageAreaCurrentChanged(int index)
{
	auto page_area = sender_cast(PageArea);
	if (!page_area) return;

	m_currentPageArea = page_area;

	if (index < 0) {
		m_meter->clear();
		return;
	}

	auto editor = qobject_cast<Editor*>(m_currentPageArea->widgetAt(index));
	if (!editor) return;
	
	m_meter->setCurrentEditor(editor);
}

void Window::onDocumentModificationChanged(bool changed)
{
	auto document = sender_cast(Document);
	if (!document) return;

	auto page_index = pageIndexOf(document);
	if (!page_index.isValid()) return;

	page_index.area->setTabFlagged(page_index.i, changed);
}

void Window::onEditorTextChanged()
{
	auto editor = sender_cast(Editor);
	if (!editor) return;

	// Eventually, call separate functions when multiple actions are needed for one slot

	auto document = qobject_cast<Document*>(editor->document());
	if (document->hasTitle()) return;

	auto page_index = pageIndexOf(document);
	if (!page_index.isValid()) return;

	auto block = document->firstBlock().text();
	page_index.area->setPageTabText(page_index.i, block.left(30));
}
