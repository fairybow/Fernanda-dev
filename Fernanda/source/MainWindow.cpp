#include "MainWindow.h"

MainWindow::MainWindow(const char* name, QWidget* parent)
	: QMainWindow(parent)
{
	setObjectName(name);
	setGeometry(0, 0, 1000, 600);
	Layout::setCentralWidget(this, m_splitter);
	setMenuBar(m_menuBar);
	setStatusBar(m_statusBar);
	m_menuBar->makeSubmenus();
	m_statusBar->addPermanentWidget(m_meter, 0);
	connect(m_splitter, &Splitter::askWindowSize, this, [&]() { return geometry(); });
	m_splitter->initialize({ 0.2, 0.4, 0.4 }, 1);

	// testing
	auto button = new QPushButton;
	m_statusBar->addPermanentWidget(button, 0);
	connect(button, &QPushButton::pressed, this, [&]() { emit testSignal1(); });
	connect(this, &MainWindow::testSignal1, this, [&]() { emitAndSave(&MainWindow::testSignal2, true); });
}

void MainWindow::connections()
{
	//
}

void MainWindow::treeViewConnections()
{
	//
}

void MainWindow::editorConnections()
{
	//
}

void MainWindow::previewConnections()
{
	//
}
