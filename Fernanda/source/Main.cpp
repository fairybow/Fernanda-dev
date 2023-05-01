#include "LaunchCop.hpp"
#include "MainWindow.h"

#include <QApplication>

int main(int argc, char* argv[])
{
	auto window_name = "MainWindow";
	LaunchCop launch_cop("Fernanda", window_name);
	if (launch_cop.exists())
		return 0;

	QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
	QApplication::setDesktopSettingsAware(true);

	QApplication fernanda(argc, argv);
	MainWindow main_window(window_name);
	main_window.show();
	return fernanda.exec();
}
