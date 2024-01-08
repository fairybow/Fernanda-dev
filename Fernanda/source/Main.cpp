#include "StartCop.hpp"
#include "Fernanda.h"

#include <QApplication>

int main(int argc, char* argv[])
{
	StartCop guard("Fernanda");
	if (guard.isRunning())
		return 0;

	QApplication application(argc, argv);
	auto is_dev = application.arguments().contains("-dev");

	Fernanda fernanda(is_dev);
	fernanda.newWindow();
	auto test_win = fernanda.newWindow();
	test_win->setGeometry(test_win->x() + 50, test_win->y() + 50, test_win->width(), test_win->height());

	return application.exec();
}
