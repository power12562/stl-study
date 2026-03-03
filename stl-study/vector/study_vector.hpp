#pragma once
#include <numeric>
#include <bit>
#include <utility>
#include "common/memory.h"

namespace mst
{
	template <typename T>
	class vector
	{
		using element_type = T;
		using container_type = vector<T>;
		static constexpr size_t max_capacity = std::numeric_limits<size_t>::max();
	public:
		template <bool CONST_ITER>
		class iterator_template
		{
			using iter_element_type = std::conditional_t<CONST_ITER, const element_type, element_type>;
			using iter_type = iterator_template<CONST_ITER>;
			using iterator = iterator_template<false>;
			using const_iterator = iterator_template<true>;
			friend class container_type;
			template <bool> friend class iterator_template;
		public:
			iterator_template() = default;
			iterator_template(iter_element_type* elementPointer) : _currentPointer(elementPointer) {}
			~iterator_template() noexcept = default;

			template <bool rhsConst>
			iterator_template(const iterator_template<rhsConst>& rhs) requires (CONST_ITER && !rhsConst) : _currentPointer(rhs._currentPointer) {}

			iter_element_type& operator*() const noexcept 
			{
				return *_currentPointer;
			}

			iter_element_type* operator->() const noexcept
			{
				return _currentPointer;
			}

			iter_element_type& operator[](size_t index) const noexcept
			{
				return _currentPointer[index];
			}

			iter_type operator+(size_t offset) const
			{
				iter_element_type* newOffset = _currentPointer + offset;
				return iter_type(newOffset);
			}

			iter_type operator++() 
			{
				_currentPointer += 1;
				return *this;
			}

			iter_type operator++(int) 
			{
				iter_type temp = *this;
				_currentPointer += 1;
				return temp;
			}

			iter_type operator-(size_t offset) const
			{
				iter_element_type* newOffset = _currentPointer - offset;
				return iter_type(newOffset);
			}

			std::ptrdiff_t operator-(iter_type rhs) const
			{
				return _currentPointer - rhs._currentPointer;
			}

			iter_type operator--()
			{
				_currentPointer -= 1;
				return *this;
			}

			iter_type operator--(int)
			{
				iter_type temp = *this;
				_currentPointer -= 1;
				return temp;
			}

			template <bool rhsConst> bool operator==(const iterator_template<rhsConst>& rhs) const
			{
				return _currentPointer == rhs._currentPointer;
			}

			template <bool rhsConst> bool operator<(const iterator_template<rhsConst>& rhs) const
			{
				return _currentPointer < rhs._currentPointer;
			}

			template <bool rhsConst> bool operator<=(const iterator_template<rhsConst>& rhs) const
			{
				return _currentPointer <= rhs._currentPointer;
			}

			template <bool rhsConst> bool operator>(const iterator_template<rhsConst>& rhs) const
			{
				return _currentPointer > rhs._currentPointer;
			}

			template <bool rhsConst> bool operator>=(const iterator_template<rhsConst>& rhs) const
			{
				return _currentPointer >= rhs._currentPointer;
			}

		private:
			iter_element_type* _currentPointer = nullptr;
		};
		using iterator = iterator_template<false>;
		using const_iterator = iterator_template<true>;

		friend void swap(container_type& lhs, container_type& rhs) noexcept
		{
			using std::swap;
			swap(lhs._memory, rhs._memory);
			swap(lhs._capacity, rhs._capacity);
			swap(lhs._size, rhs._size);
		}

		size_t size() const
		{
			return _size;
		}

		size_t capacity() const
		{
			return _capacity;
		}

		bool empty() const
		{
			return _size == 0 ? true : false;
		}

		void resize(size_t size)
		{
			if (_size != size)
			{
				size_t prevSize = _size;
				size_t newSize = size;
				if (_size < size)
				{
					if (_capacity < newSize)
					{
						reserve(newSize);
					}

					if constexpr (std::is_trivially_copyable_v<element_type> == false)
					{
						for (size_t i = prevSize; i < newSize; i++)
						{
							std::construct_at(&_memory[i]);
						}
					}
				}
				else
				{
					if constexpr (std::is_trivially_copyable_v<element_type> == false)
					{
						for (size_t i = newSize; i < prevSize; i++)
						{
							std::destroy_at(&_memory[i]);
						}
					}
				}
				_size = newSize;
			}	
		}

		void reserve(size_t capacity)
		{
			if (_capacity < capacity)
			{	
				size_t size = _size;
				size_t newCapacity = capacity;
				size_t prevCapacity = _capacity;
				element_type* prevMemory = _memory;
				element_type* newMemory = NewMem(newCapacity);
				SafeMove(newMemory, prevMemory, size);
				SafeFree(prevMemory);

				_capacity = newCapacity;
				_memory = newMemory;		
			}
		}

		iterator begin()
		{
			return iterator(_memory);
		}

		iterator end()
		{
			element_type* newOffset = _memory + _size;
			return iterator(newOffset);
		}

		const_iterator begin() const
		{
			return const_iterator(_memory);
		}

		const_iterator end() const
		{
			element_type* newOffset = _memory + _size;
			return const_iterator(newOffset);
		}

		const_iterator cbegin() const
		{
			return const_iterator(_memory);
		}

		const_iterator cend() const
		{
			element_type* newOffset = _memory + _size;
			return const_iterator(newOffset);
		}

		void clear() noexcept
		{
			FreeMem(_memory, _size);
			_memory = nullptr;
			_size = 0;
			_capacity = 0;
		}

		template<typename VT>
		void push_back(VT&& value)
		{
			size_t lastIndex = _size;
			if (_capacity <= lastIndex)
			{
				size_t newCapacity = GetNewCapacitySize();
				reserve(newCapacity);
			}
			std::construct_at(&_memory[lastIndex], std::forward<VT>(value));
			++_size;
		}

		void pop_back()
		{
			size_t lastIndex = _size - 1;
			std::destroy_at(&_memory[lastIndex]);
			--_size;
		}

		template<typename VT>
		iterator insert(iterator pos, VT&& value)
		{
			size_t index = pos - begin();
			if (0 < size())
			{
				size_t lastIndex = size();
				if (_capacity <= lastIndex)
				{
					size_t newCapacity = GetNewCapacitySize();
					reserve(newCapacity);
				}

				if constexpr (std::is_trivially_copyable_v<element_type>)
				{
					size_t dstStart = index + 1;
					size_t cpyCount = lastIndex - index;
					std::memmove(&_memory[dstStart], &_memory[index], sizeof(element_type) * cpyCount);
				}
				else
				{
					std::construct_at(&_memory[lastIndex], std::move(_memory[lastIndex - 1]));
					for (size_t i = lastIndex - 1; i > index; --i)
					{
						_memory[i] = std::move(_memory[i - 1]);
					}
				}
				_memory[index] = std::forward<VT>(value);
				++_size;
			}
			else
			{
				push_back(std::forward<VT>(value));
			}
			return begin() + index;
		}

		iterator insert(iterator pos, std::initializer_list<element_type> list)
		{
			size_t index = pos - begin();
			size_t listSize = list.size();
			size_t newSize = size() + listSize;
			size_t lastIndex = size() - 1;
			if (_capacity < newSize)
			{
				reserve(newSize);
			}

			if (0 < size())
			{		
				if constexpr (std::is_trivially_copyable_v<element_type>)
				{
					size_t dstIndex = index + listSize;
					size_t cpyCount = size() - index;
					std::memmove(&_memory[dstIndex], &_memory[index], sizeof(element_type) * cpyCount);
					std::memcpy(&_memory[index], list.begin(), sizeof(element_type) * listSize);
				}
				else
				{
					size_t newLastIndex = newSize - 1;
					for (size_t i = newLastIndex; i > lastIndex; --i)
					{
						if (i >= listSize)
						{
							size_t cpyIndex = i - listSize;
							if (index <= cpyIndex)
							{
								std::construct_at(&_memory[i], std::move(_memory[cpyIndex]));
								continue;
							}
						}
						auto listIter = list.begin();
						std::construct_at(&_memory[i], listIter[i - index]);
					}

					auto listIter = list.begin();
					for (size_t i = index; i <= lastIndex; ++i)
					{
						_memory[i] = listIter[i - index];
					}
				}
			}
			else
			{
				if constexpr (std::is_trivially_copyable_v<element_type>)
				{
					std::memcpy(&_memory[0], list.begin(), sizeof(element_type) * listSize);
				}
				else
				{
					auto listIter = list.begin();
					for (size_t i = 0; i < listSize; ++i)
					{
						std::construct_at(&_memory[i], listIter[i]);
					}
				}		
			}
			_size = newSize;
			return begin() + index;
		}

		iterator erase(iterator pos)
		{
			size_t index = pos - begin();
			size_t lastIndex = size() - 1;

			if constexpr (std::is_trivially_copyable_v<element_type>)
			{
				size_t cpyStart = index + 1;
				size_t cpyCount = size() - cpyStart;
				std::memmove(&_memory[index], &_memory[cpyStart], sizeof(element_type) * cpyCount);
			}
			else
			{
				for (size_t i = index; i < lastIndex; ++i)
				{
					_memory[i] = std::move(_memory[i + 1]);
				}
				std::destroy_at(&_memory[lastIndex]);
			}
			--_size;

			return begin() + index;
		}

		element_type& operator[] (size_t index) noexcept
		{
			return _memory[index];
		}

		const element_type& operator[] (size_t index) const noexcept
		{
			return _memory[index];
		}

		container_type& operator=(container_type rhs)
		{
			swap(*this, rhs);
			return *this;
		}

		explicit vector(const size_t& size = 0)
		{
			if (size != 0)
			{
				resize(size);
			}
		}

		vector(std::initializer_list<element_type> list)
		{
			size_t newSize = list.size();
			if (0 < newSize)
			{
				reserve(newSize);
				for (auto& item : list)
				{
					push_back(item);
				}
			}
		}

		vector(const container_type& rhs)
		{
			reserve(rhs.capacity());
			if constexpr (std::is_trivially_copyable_v<element_type>)
			{
				std::memcpy(_memory, rhs._memory, sizeof(element_type) * rhs.size());
			}
			else
			{
				size_t size = rhs.size();
				for (size_t i = 0; i < size; ++i)
				{
					std::construct_at(&_memory[i], rhs[i]);
				}
			}
			_size = rhs.size();
		}

		vector(container_type&& rhs) noexcept
		{
			swap(*this, rhs);
		}

		~vector() noexcept
		{
			clear();
		}

	private:
		element_type* _memory = nullptr;
		size_t _size = 0;
		size_t _capacity = 0;

		size_t GetNewCapacitySize()
		{
			if (_capacity == 0)
			{
				return 1;
			}

			size_t bitCapacity = std::bit_ceil(_capacity);
			constexpr size_t half_capacity = max_capacity / 2;
			if (bitCapacity > half_capacity)
			{
				return max_capacity;
			}
			return bitCapacity * 2;
		}

		static element_type* NewMem(size_t size)
		{
			if (0 < size)
			{
				size_t mallocSize = sizeof(element_type) * size;
				void* mem = malloc(mallocSize);
				return static_cast<element_type*>(mem);
			}	
			return nullptr;
		}

		static void FreeMem(element_type* memory, size_t size)
		{
			for (size_t i = 0; i < size; i++)
			{
				std::destroy_at(&memory[i]);
			}
			SafeFree(memory);
		}
	};

}