#pragma once

#include <QDebug>
#include <QPlainTextEdit>

class Editor : public QPlainTextEdit
{
	Q_OBJECT

public:
	Editor(QWidget* parent = nullptr);
	~Editor() { qDebug() << __FUNCTION__; }
};
