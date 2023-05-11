#pragma once

namespace Regex
{
	constexpr char URL_BEGINNING[] = \
		"(https:\\/\\/|www.)";

	constexpr char LEADING_WHITESPACE[] = \
		"(\\s|\\n|\\r|\U00002029|^)+";
}
