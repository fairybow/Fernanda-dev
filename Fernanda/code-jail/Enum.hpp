#pragma once

#include <QDebug>
#include <QString>

#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#define ENUM_REFLECTOR(Name, ...) \
	enum class Name : int { \
		__VA_ARGS__, \
		END \
	}; \
	\
	class Name##Reflector \
	{ \
	public: \
		static std::string valueString(Name value) \
		{ \
			return strings()[static_cast<int>(value)]; \
		} \
		\
		static QString valueQString(Name value) \
		{ \
			return QString::fromStdString(valueString(value)); \
		} \
		\
		static std::string toString(Name value) \
		{ \
			std::string enum_name = #Name; \
			\
			return enum_name + "::" + valueString(value); \
		} \
		\
		static QString toQString(Name value) \
		{ \
			return QString::fromStdString(toString(value)); \
		} \
		\
	private: \
		static std::vector<std::string> strings() \
		{ \
			static std::vector<std::string> strings; \
			static auto whitespace = " \t\r\n"; \
			\
			if (strings.empty()) { \
				std::string string = #__VA_ARGS__; \
				auto length = string.length(); \
				std::stringstream stream(string); \
				std::string item; \
				\
				while (std::getline(stream, item, ',')) { \
					auto start = item.find_first_not_of(whitespace); \
					auto end = item.find_last_not_of(whitespace); \
					if (start != std::string::npos && end != std::string::npos) \
						strings.push_back(item.substr(start, end - start + 1)); \
				} \
			} \
			\
			return strings; \
		} \
	}; \
	\
	friend std::ostream& operator<<(std::ostream& os, Name value) \
	{ \
		os << Name##Reflector::toString(value); \
		\
		return os; \
	} \
	\
	friend QDebug operator<<(QDebug debug, Name value) \
	{ \
		debug.nospace() << Name##Reflector::toString(value); \
		\
		return debug.maybeSpace(); \
	}
