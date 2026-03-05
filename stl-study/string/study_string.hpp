#pragma once
#include <string.h>

namespace mst
{
	template <typename T>
	struct char_traits;

	template<>
	struct char_traits<char>
	{
		using char_t = char;
		static size_t length(const char_t* str)
		{
			return strlen(str);
		}

		static int compare(const char_t* str1, const char_t* str2, size_t count)
		{
			return memcmp(str1, str2, count);
		}

		static void copy(char_t* dst, const char_t* src, size_t dstSize)
		{
			memcpy(dst, src, sizeof(char_t) * dstSize);
		}

		static void move(char_t* dst, const char_t* src, size_t dstSize)
		{
			memmove(dst, src, sizeof(char_t) * dstSize);
		}

		static void assing(char_t* dst, char_t c, size_t count)
		{
			memset(dst, c, count);		
		}
	};
}
