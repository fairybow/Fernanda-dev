#include "MainWindow.h"
#include "StartCop.hpp"

#include <QApplication>

int main(int argc, char *argv[])
{
	StartCop x("Fernanda");
	if (x.exists())
		return 0;

	QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
	QApplication::setDesktopSettingsAware(true);
	QApplication fernanda(argc, argv);
	MainWindow main_window("MainWindow");
	main_window.show();
	return fernanda.exec();
}
