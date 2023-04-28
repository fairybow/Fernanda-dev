#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	//setObjectName(name);
	setGeometry(0, 0, 1000, 600);
	Layout::setCentralWidget(this, m_splitter);
	setMenuBar(m_menuBar);
	setStatusBar(m_statusBar);
	m_menuBar->makeSubmenus();
	m_statusBar->addPermanentWidget(m_meter, 0);
	connect(m_splitter, &Splitter::askWindowSize, this, [&]() { return geometry(); });
	m_splitter->initialize({ 0.2, 0.4, 0.4 }, 1);
}
