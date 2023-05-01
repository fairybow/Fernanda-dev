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

	auto button_1 = new QPushButton;
	auto button_2 = new QPushButton;
	button_1->setText("Save");
	button_2->setText("Load");
	m_statusBar->addPermanentWidget(button_1, 0);
	m_statusBar->addPermanentWidget(button_2, 0);
	connect(button_1, &QPushButton::pressed, this, [&]() { emit testSignal1(); });
	connect(button_2, &QPushButton::pressed, this, [&]() { emit testSignal2(); });
	connect(this, &MainWindow::testSignal1, this, [&]()
		{
			emitAndSave(&MainWindow::testSignal3, QString("Signal received by m_settings"));
			emitAndSave(&MainWindow::testSignal4, 80085, this);
		});
	connect(this, &MainWindow::testSignal2, this, [&]()
		{
			// load both and print to console
		});
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
