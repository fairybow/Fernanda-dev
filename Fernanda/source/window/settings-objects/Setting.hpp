#pragma once

#include <QVariant>

#include <functional>

template <typename TSubject>
class Setting
{
public:
	Setting() = default;

	template <typename T>
	Setting(const T& value, std::function<void(TSubject)> setAction)
		: m_variant(QVariant::fromValue<T>(value)), m_setAction(setAction)
	{}

	template <typename T, typename TClass>
	Setting(const T& value, TClass* context, void (TClass::* setAction)(TSubject))
		: m_variant(QVariant::fromValue<T>(value)), m_setAction(lambdaWrap<TClass>(context, setAction))
	{}

	QVariant variant() const
	{
		return m_variant;
	}

	void setVariant(const QVariant& variant)
	{
		m_variant = variant;
	}

	void set(TSubject subject)
	{
		m_setAction(subject);
	}

	template <typename T>
	T value()
	{
		return m_variant.value<T>();
	}

	template <typename T>
	void setValue(const T& value)
	{
		m_variant = QVariant::fromValue<T>(value);
	}

private:
	QVariant m_variant;
	std::function<void(TSubject)> m_setAction;
	//QList<QVariant> m_pool;

	template <typename TClass>
	auto lambdaWrap(TClass* context, void (TClass::* setAction)(TSubject))
	{
		return [=](TSubject subject) { std::invoke(setAction, context, subject); };
	}
};
