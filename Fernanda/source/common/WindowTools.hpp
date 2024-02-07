#pragma once

#include <QApplication>
#include <QList>
#include <QMainWindow>
#include <QStyle>

#include <type_traits>

namespace WindowTools
{
	namespace
	{
		//
	}

	inline void moveXYIfTaken(QMainWindow* window, QList<QMainWindow*>& windows)
	{
		for (auto& other : windows) {
			if (other == window) continue;

			auto x = window->x();
			auto y = window->y();

			auto distance = 6;
			auto title_bar_height = QApplication::style()->pixelMetric(QStyle::PM_TitleBarHeight);

			if (x == other->x() /*&& y == other->y()*/) // <- I forget why `y` didn't work/matter
				window->move(x + distance + title_bar_height, y + distance);
		}
	}

	template <typename T>
	inline std::enable_if_t<std::is_base_of_v<QMainWindow, T>, void>
		moveXYIfTaken(T* window, QList<T*>& windows)
	{
		moveXYIfTaken(static_cast<QMainWindow*>(window), reinterpret_cast<QList<QMainWindow*>&>(windows));
	}
}
