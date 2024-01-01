/*

// Window

// Testing
QTimer::singleShot(0, [=]() {

	auto x = 6;
	for (auto i = 0; i < x; ++i) {
		auto suffix = QString::number(i);
		auto name = QString("editor_") + suffix;
		auto editor = new QPlainTextEdit(this);
		editor->setPlainText(suffix.repeated(10));
		qDebug() << editor->parent() << name + " intial parent";
		auto index = pageArea->add(editor, name);
		qDebug() << editor->parent() << name + " new parent";

		qDebug() << index << " new page index";
		qDebug() << pageArea->currentIndex() << " page area current index";

		pageArea->setCurrentIndex(index);
	}

	});

// Main

#include "common/Io.hpp"

QObject::connect(&guard, &StartCop::launched, &fernanda, [&] {
	qDebug() << __FUNCTION__;
	});

// Connect to a MainWindow signal that requests a new window?

Window window2test(&fernanda);
window2test.show();

// Testing below

QObject::connect(&window, &Window::treeViewFileDoubleClicked, &fernanda, [&](const Path& path) {

	qDebug() << Io::read(path);

	});

// PageArea

qDebug() << m_tabBar->parent() << "m_tabBar parent"; // PageArea m_tabBar parent
qDebug() << m_controls->parent() << "m_controls parent"; // PageArea m_controls parent
qDebug() << m_underlay->parent() << "m_underlay parent"; // QStackedWidget m_underlay parent
qDebug() << m_mainStack->parent() << "m_mainStack parent"; // PageArea m_mainStack parent
qDebug() << m_widgetsStack->parent() << "m_widgetsStack parent"; // QStackedWidget m_widgetsStack parent
qDebug() << layout() << "PageArea layout"; // QVBoxLayout PageArea layout

qDebug() << "Tab bar parent:" << m_tabBar->parent(); // PageArea
qDebug() << "Close button parent:" << close_button->parent(); // TabBar

*/
