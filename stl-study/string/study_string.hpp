#pragma once
#include <string.h>
#include <cwchar>
#include <iostream>
#include <type_traits>
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

		static void copy(char_t* dst, const char_t* src, size_t count)
		{
			memcpy(dst, src, count);
		}

		static void move(char_t* dst, const char_t* src, size_t count)
		{
			memmove(dst, src, count);
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

		static void copy(char_t* dst, const char_t* src, size_t count)
		{
			wmemcpy(dst, src, count);
		}

		static void move(char_t* dst, const char_t* src, size_t count)
		{
			wmemmove(dst, src, count);
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
		using traits_t = _traits_t;
		using ostream_t = std::conditional_t<std::is_same_v<char, char_t>, std::ostream, std::wostream>;

		static constexpr size_t STACK_BUF_SIZE  = 32;
		static constexpr size_t STACK_BUF_COUNT = STACK_BUF_SIZE / sizeof(char_t);
		static constexpr size_t STACK_MAX_LEN   = STACK_BUF_COUNT - 1;
		struct stack_buffer
		{
			char_t _str[STACK_BUF_SIZE];
		};
		struct heap_buffer
		{
			char_t* _str;
			size_t  _capacity;
		};

	public:
		friend void swap(basic_string& lhs, basic_string& rhs)
		{
			using std::swap;
			swap(lhs._stack._str, rhs._stack._str);
			swap(lhs._size, rhs._size);
		}

		friend ostream_t& operator<<(ostream_t& out, const basic_string& str)
		{
			out.write(str.c_str(), str.length());
			return out;
		}
		
		~basic_string() 
		{
			delete_heap();
		}

		basic_string()
		{
			reset_stack();
		}

		basic_string(const char_t* str)
		{
			reset_stack();
			size_t len = traits_t::length(str);
			cpy_mem(str, len);
		}

		basic_string(const basic_string& rhs)
		{
			reset_stack();
			size_t len = rhs.length();
			cpy_mem(rhs.c_str(), len);
		}

		basic_string(basic_string&& rhs) noexcept
		{
			reset_stack();
			swap(*this, rhs);
		}

		size_t size() const noexcept { return _size; }

		size_t length() const noexcept { return _size; }

		bool empty() const noexcept { return 0 == size(); }

		void clear() noexcept
		{
			char_t* str = data();
			str[0] = '\0';
			_size = 0;
		}

		size_t capacity() const noexcept
		{
			if (heap_flag() == false)
			{
				return STACK_BUF_COUNT;			
			}
			else
			{
				return _heap._capacity;
			}
		}

		char_t* data() noexcept
		{
			if (heap_flag() == false)
			{
				return _stack._str;
			}
			else
			{
				return _heap._str;
			}
		}

		const char_t* data() const noexcept
		{
			if (heap_flag() == false)
			{
				return _stack._str;
			}
			else
			{
				return _heap._str;
			}
		}

		const char_t* c_str() const noexcept
		{
			return data();
		}

		void reserve(size_t count)
		{
			if (count < capacity())
				return;

			size_t newCapacity = align_up(count + 1);
			char_t* newStr = new char_t[newCapacity];
			char& heapFlag = heap_flag();
			if (heapFlag == false)
			{
				if (empty() == false)
				{
					traits_t::copy(newStr, _stack._str, _size);
				}
				heapFlag = true;
			}	
			else
			{
				if (empty() == false)
				{
					traits_t::copy(newStr, _heap._str, _size);
				}
				delete[] _heap._str;
			}
			newStr[_size] = '\0';
			_heap._str = newStr;
			_heap._capacity = newCapacity;
		}

		basic_string& append(const char_t* str)
		{
			size_t srcLen = traits_t::length(str);
			size_t newLen = length() + srcLen;
			reserve(newLen + (newLen >> 1));

			char_t* buffer = data();
			traits_t::copy(&buffer[_size], str, srcLen);
			_size = newLen;
			buffer[_size] = '\0';

			return *this;
		}

		basic_string& append(const basic_string& str)
		{
			size_t srcLen = str.length();
			size_t newLen = length() + srcLen;
			reserve(newLen + (newLen >> 1));

			char_t* buffer = data();
			traits_t::copy(&buffer[_size], str.c_str(), srcLen);
			_size = newLen;
			buffer[_size] = '\0';

			return *this;
		}

		basic_string& operator=(basic_string rhs) noexcept
		{
			swap(*this, rhs);
			return *this;
		}

		char_t& operator[](size_t i) noexcept
		{
			return data()[i];
		}

		const char_t& operator[](size_t i) const noexcept
		{
			return data()[i];
		}

		basic_string& operator+=(const basic_string& rhs)
		{
			return append(rhs);
		}

		basic_string& operator+=(const char_t* rhs)
		{
			return append(rhs);
		}

	private:
		union
		{
			stack_buffer _stack;
			heap_buffer _heap;
		};
		size_t _size = 0;

		void reset_stack()
		{
			std::memset(_stack._str, 0, STACK_BUF_SIZE);
		}

		void delete_heap()
		{
			if (heap_flag())
			{
				delete[] _heap._str;
				_heap._capacity = 0;
				reset_stack();
			}
		}

		char& heap_flag() noexcept
		{
			constexpr size_t LAST_BIT_INDEX = STACK_BUF_SIZE - 1;
			char* stackStr = reinterpret_cast<char*>(_stack._str);
			return stackStr[LAST_BIT_INDEX];
		}

		const char& heap_flag() const noexcept
		{
			constexpr size_t LAST_BIT_INDEX = STACK_BUF_SIZE - 1;
			const char* stackStr = reinterpret_cast<const char*>(_stack._str);
			return stackStr[LAST_BIT_INDEX];
		}

		static bool is_stack_len(size_t len) noexcept
		{
			return len < STACK_MAX_LEN;
		}

		void cpy_mem(const char_t* src, size_t count)
		{
			if (is_stack_len(count))
			{
				traits_t::copy(_stack._str, src, count);
				_stack._str[count] = '\0';
			}
			else
			{
				reserve(count);
				traits_t::copy(_heap._str, src, count);
				_heap._str[count] = '\0';
			}
			_size = count;
		}

		static constexpr size_t align_up(size_t n) noexcept
		{
			// STACK_BUF_SIZE의 배수로
			return (n + (STACK_BUF_SIZE - 1)) & ~(STACK_BUF_SIZE - 1);
		}

	};

	using string = basic_string<char>;
	using wstring = basic_string<wchar_t>;
}
