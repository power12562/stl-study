#pragma once
#include <string.h>
#include <cwchar>
#include "common/memory.h"

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
			memcpy(dst, src, dstSize);
		}

		static void move(char_t* dst, const char_t* src, size_t dstSize)
		{
			memmove(dst, src, dstSize);
		}

		static void assing(char_t* dst, char_t c, size_t count)
		{
			memset(dst, c, count);		
		}
	};

	template<>
	struct char_traits<wchar_t>
	{
		using char_t = wchar_t;
		static size_t length(const char_t* str)
		{
			return wcslen(str);
		}

		static int compare(const char_t* str1, const char_t* str2, size_t count)
		{
			return wmemcmp(str1, str2, count);
		}

		static void copy(char_t* dst, const char_t* src, size_t dstSize)
		{
			wmemcpy(dst, src, dstSize);
		}

		static void move(char_t* dst, const char_t* src, size_t dstSize)
		{
			wmemmove(dst, src, dstSize);
		}

		static void assing(char_t* dst, char_t c, size_t count)
		{
			wmemset(dst, c, count);
		}
	};

	template <typename _char_t, typename _traits_t = char_traits<_char_t>>
	class basic_string
	{
		using char_t = _char_t;
		static constexpr size_t STACK_BUF_SIZE  = 32;
		static constexpr size_t STACK_BUF_COUNT = STACK_BUF_SIZE / sizeof(char_t);
		struct stack_buffer
		{
			char_t _str[STACK_BUF_SIZE];
		};
		struct heap_buffer
		{
			size_t  _capacity;
			char_t* _str;
		};

	public:
		basic_string()
		{
			std::memset(_stack._str, 0, STACK_BUF_SIZE);
		}

	private:
		union
		{
			stack_buffer _stack;
			heap_buffer _heap;
		};
		size_t _size = 0;

		char& stack_flag() 
		{
			constexpr size_t LAST_BIT_INDEX = STACK_BUF_SIZE - 1;
			char* stackStr = reinterpret_cast<char*>(_stack._str);
			return stackStr[LAST_BIT_INDEX];
		}
	};

	using string = basic_string<char>;
	using wstring = basic_string<wchar_t>;
}
