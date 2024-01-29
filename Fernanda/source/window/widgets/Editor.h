#pragma once

#include <QDebug>
#include <QPlainTextEdit>

class Editor : public QPlainTextEdit
{
	Q_OBJECT

public:
	Editor(QWidget* parent = nullptr);
	~Editor() { qDebug() << __FUNCTION__; }

	bool isTypewriter() const;
	void setIsTypewriter(bool isTypewriter);

private:
	bool m_isTypewriter = false;

private slots:
	void typewriter();
};
