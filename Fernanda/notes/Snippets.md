### User
```
template<typename T>
inline void save(T value)
{
	qDebug() << value;
}

template<typename... Args>
inline void save(Args... args)
{
	(qDebug() << ... << args);
}
```

### MainWindow
```
template<typename... Args>
struct SignalArgs
{
	std::tuple<Args...> args;
	SignalArgs(Args... args)
		: args(std::make_tuple(args...)) {}
};

template<typename T>
inline void emitAndSave(void (MainWindow::* signal)(T), T value)
{
	emit(this->*signal)(value);
	m_user->save(value);
}

template<typename T, typename U>
inline void emitAndSave(void (MainWindow::* signal)(T), T value, U* pointerCheck)
{
	if (pointerCheck == nullptr) return;
	emit(this->*signal)(value);
	m_user->save(value);
}

template<typename... Args>
inline void emitAndSave(void (MainWindow::* signal)(Args...), SignalArgs<Args...> signalArgs)
{
	std::apply([&, signal](Args... args) { emit(this->*signal)(args...); }, signalArgs.args);
	std::apply([&](Args... args) { m_user->save(args...); }, signalArgs.args);
}

//

auto button = new QPushButton;
button->setText("Signal");
m_statusBar->addPermanentWidget(button, 0);
connect(button, &QPushButton::pressed, this, [&]() { emit testSignal1(); });
connect(this, &MainWindow::testSignal1, this, [&]()
	{
		emitAndSave(&MainWindow::testSignal2, QString("Signal received by m_settings"));
		emitAndSave(&MainWindow::testSignal3, 666, this);
	});
```