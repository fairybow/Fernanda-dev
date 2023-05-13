#include "common/Path.hpp"
#include "MainWindow.h"

#include <QApplication>
#include <QFont>
#include <QGuiApplication>

#include <filesystem>

void setFont(QApplication& application);
void ensureVisible(QGuiApplication& application, QMainWindow& mainWindow);

int main(int argc, char* argv[])
{
	QApplication::setHighDpiScaleFactorRoundingPolicy(
		Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
	QApplication::setDesktopSettingsAware(true);
	QApplication fernanda(argc, argv);

	std::filesystem::path open_file;
	for (auto& arg : fernanda.arguments())
		if (arg.endsWith(".txt")) // handle projects, too
			open_file = Path::toStdFs(arg);

	MainWindow main_window("MainWindow", fernanda.arguments().contains("-dev"), open_file);
	setFont(fernanda);
	main_window.show();
	ensureVisible(fernanda, main_window);
	return fernanda.exec();
}

void setFont(QApplication& application)
{
	auto font = application.font();
	font.setStyleStrategy(QFont::PreferAntialias);
	font.setHintingPreference(QFont::HintingPreference::PreferNoHinting);
	font.setPointSizeF(9);
	application.setFont(font);
}

void ensureVisible(QGuiApplication& application, QMainWindow& mainWindow)
{
	auto screens = QGuiApplication::screens();
	if (screens.isEmpty()) return;
	auto visible = false;
	for (auto& screen : screens) {
		auto rect = screen->geometry();
		if (rect.contains(mainWindow.geometry())) {
			visible = true;
			break;
		}
	}
	if (visible) return;
	auto rect = screens.first()->geometry();
	mainWindow.move(rect.topLeft());
}
