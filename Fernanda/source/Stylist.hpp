#pragma once

#include <QVector>
#include <QWidget>

class Stylist : public QObject
{
public:
	inline Stylist(QObject* parent = nullptr)
		: QObject(parent) {}
};
