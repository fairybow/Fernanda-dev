#include "MainWindow.h"
#include "StartCop.hpp"

#include <QApplication>

int main(int argc, char* argv[])
{
	auto window_name = "MainWindow";
	StartCop start_cop("Fernanda", window_name);
	if (start_cop.exists())
		return 0;

	QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
	QApplication::setDesktopSettingsAware(true);

	QApplication fernanda(argc, argv);
	MainWindow main_window(window_name);
	main_window.show();
	return fernanda.exec();
}
