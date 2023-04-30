#pragma once

#include <QSettings>

class Settings : public QObject
{
public:
	inline Settings(QObject* parent = nullptr)
		: QObject(parent) {}
};
