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
	Setting(const T& value, TClass* context, void (TClass::* memberFuncPtr)(TSubject))
		: Setting(value, lambdaWrap<TClass>(context, memberFuncPtr))
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
		if (m_setAction)
			m_setAction(subject);
	}

	template <typename T>
	T value() const
	{
		return m_variant.value<T>();
	}

	template <typename T>
	void setValue(const T& value)
	{
		m_variant = QVariant::fromValue<T>(value);
	}

private:
	QVariant m_variant = QVariant();
	std::function<void(TSubject)> m_setAction = std::function<void(TSubject)>();
	//QList<QVariant> m_pool;

	template <typename TClass>
	auto lambdaWrap(TClass* context, void (TClass::* memberFuncPtr)(TSubject))
	{
		return [=](TSubject subject) { (context->*memberFuncPtr)(subject); };
	}
};
