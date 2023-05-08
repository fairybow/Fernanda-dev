### Path
```
struct QFsPath
{
	using StdFsPath = std::filesystem::path;
	StdFsPath path;

	enum class Type { QString, StdFs };

	inline QFsPath() : path() {}
	inline QFsPath(const QString& qStringPath) : path(qStringPath.toStdString()) {}
	inline QFsPath(const StdFsPath& stdFsPath) : path(stdFsPath) {}
	inline QFsPath(const char* cStringPath) : path(cStringPath) {}

	inline operator StdFsPath() const { return path; }

	QFsPath operator/(const StdFsPath& rhs) const
	{
		return QFsPath(path / rhs);
	}

	QFsPath operator/(const QString& rhs) const
	{
		return QFsPath(path / StdFsPath(rhs.toStdString()));
	}

	QFsPath operator/(const char* rhs) const
	{
		return QFsPath(path / StdFsPath(rhs));
	}

	inline QString toQString(bool sanitize = true) const
	{
		return Path::toQString(path, sanitize);
	}

	template<typename T>
	inline T name(bool keepExtension = false, Type type = Type::QString)
	{
		T name_value{};
		switch (type) {
		case Type::QString:
			name_value = Path::qStringName(path, keepExtension);
			break;
		case Type::StdFs:
			name_value = Path::name(path, keepExtension);
			break;
		}
		return name_value;
	}

	inline bool has_extension() const noexcept { return path.has_extension(); }

	inline QFsPath& replace_extension(const StdFsPath& replacement = StdFsPath())
	{
		path.replace_extension(replacement);
		return *this;
	}
};
```

### User
```
template<typename T>
void save(T value)
{
	qDebug() << value;
}

template<typename... Args>
void save(Args... args)
{
	(qDebug() << ... << args);
}
```

### MainWindow
```
template<typename T, typename U>
U saveConfigPassthrough(T value, const QString& valueKey, QObject* associatedObject, std::function<U()> configurableAction)
{
	U result = configurableAction();
	m_user->save(value, valueKey, associatedObject);
	return result;
}

//

template<typename... Args>
struct SignalArgs
{
	std::tuple<Args...> args;
	SignalArgs(Args... args)
		: args(std::make_tuple(args...)) {}
};

template<typename T>
void emitAndSave(void (MainWindow::* signal)(T), T value)
{
	emit(this->*signal)(value);
	m_user->save(value);
}

template<typename T, typename U>
void emitAndSave(void (MainWindow::* signal)(T), T value, U* pointerCheck)
{
	if (pointerCheck == nullptr) return;
	emit(this->*signal)(value);
	m_user->save(value);
}

template<typename... Args>
void emitAndSave(void (MainWindow::* signal)(Args...), SignalArgs<Args...> signalArgs)
{
	std::apply([&, signal](Args... args) { emit(this->*signal)(args...); }, signalArgs.args);
	std::apply([&](Args... args) { m_user->save(args...); }, signalArgs.args);
}

//

template<typename T>
void emitAndSave(void (MainWindow::* signal)(T), T value, const QString& valueKey, QObject* namedObject = nullptr)
{
	emit(this->*signal)(value);
	QString group_prefix;
	if (namedObject)
		group_prefix = namedObject->objectName();
	m_user->save(value, valueKey, group_prefix);
}

//

auto button_1 = new QPushButton;
button_1->setText("Save");
m_statusBar->addPermanentWidget(button_1, 0);
connect(button_1, &QPushButton::pressed, this, [&] { emit testSignal1(); });
connect(this, &MainWindow::testSignal1, this, [&]
	{
		emitAndSave(&MainWindow::testSignal3, 666, "Key", this);
		emitAndSave(&MainWindow::testSignal3, 12, "Key");
	});

auto button_2 = new QPushButton;
button_2->setText("Load");
m_statusBar->addPermanentWidget(button_2, 0);
connect(button_2, &QPushButton::pressed, this, [&] { emit testSignal2(); });
connect(this, &MainWindow::testSignal2, this, [&]
	{
		auto x = loadConfig("Key", this, 666);
		auto y = loadConfig("Key", 12);
		qDebug() << x << y;
	});
	
connect(this, &MainWindow::testSignal3, this, [&]
	{
		qDebug() << "testSignal 3 emitted by MainWindow using `emitAndSave`";
	});
```
