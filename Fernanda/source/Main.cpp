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
	//fernanda.newWindow();

	return application.exec();
}
