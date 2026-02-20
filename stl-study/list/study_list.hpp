#include "common/memory.h"

namespace mst
{
	template <typename T>
	class list
	{
		using element_type = T;
		using container_type = list<element_type>;

		struct node
		{
			node(const element_type& data) : _data(data) {}
			element_type _data;
			node* _prev = nullptr;
			node* _next = nullptr;
		};
	public:

		class iterator
		{
			friend class container_type;
		public:
			iterator(node* node) : _currentNode(node) {}
			iterator(const iterator& rhs) : _currentNode(rhs._currentNode) {}
			~iterator() = default;

			element_type& operator*() noexcept
			{
				return _currentNode->_data;
			}

			element_type* operator->() noexcept
			{
				return &_currentNode->_data;
			}

			const element_type& operator*() const noexcept
			{
				return _currentNode->_data;
			}

			const element_type* operator->() const noexcept
			{
				return &_currentNode->_data;
			}

			iterator operator++()
			{
				if (_currentNode->_next == nullptr) throw std::runtime_error("next iterator is null");
				else _currentNode = _currentNode->_next;
				return *this;
			}

			iterator operator++(int)
			{
				iterator temp = *this;
				if (_currentNode->_next == nullptr) throw std::runtime_error("next iterator is null");
				else _currentNode = _currentNode->_next;
				return temp;
			}

			iterator operator--()
			{
				if (_currentNode->_next == nullptr) throw std::runtime_error("prev iterator is null");
				else _currentNode = _currentNode->_prev;
				return *this;
			}

			iterator operator--(int)
			{
				iterator temp = *this;
				if (_currentNode->_next == nullptr) throw std::runtime_error("prev iterator is null");
				else _currentNode = _currentNode->_prev;
				return temp;
			}

			bool operator==(const iterator& rhs) const noexcept
			{
				if (_currentNode != rhs._currentNode) return false;
				node* lnode = _currentNode;
				node* rnode = rhs._currentNode;
				if (lnode->_next != rnode->_next) return false;
				if (lnode->_prev != rnode->_prev) return false;
				return true;
			}

		private:
			node* _currentNode = nullptr;
		};

		list() = default;
		~list()
		{
			clear();
		}

		iterator begin()
		{
			if (_front == nullptr) throw std::runtime_error("front node is null.");
			else
			{
				return iterator(_front);
			}
		}

		iterator end()
		{
			if (_back == nullptr) throw std::runtime_error("back node is null.");
			else
			{
				return iterator(_back);
			}
		}

		element_type& front()
		{
			if (_front == nullptr) throw std::runtime_error("front node is null.");
			else return _front->_data;
		}

		const element_type& front() const
		{
			if (_front == nullptr) throw std::runtime_error("front node is null.");
			else return _front->_data;
		}

		element_type& back()
		{
			if (_back == nullptr) throw std::runtime_error("back node is null.");
			else return _back->_data;
		}

		const element_type& back() const
		{
			if (_back == nullptr) throw std::runtime_error("back node is null.");
			else return _back->_data;
		}

		template<typename VT>
		void push_front(VT&& value)
		{
			node* prevFront = _front;
			node* newFront = new node(std::forward<VT>(value));
			if (0 < _size)
			{
				link_node(newFront, prevFront);
			}
			else
			{
				_front = newFront;
				_back = newFront;
			}
			++_size;
		}

		void pop_front() noexcept
		{
			node* prevFront = _front;
			if (prevFront != nullptr)
			{
				node* newFront = _front->_next;
				if (newFront != nullptr)
				{
					newFront->_prev = nullptr;
				}
				SafeDelete(prevFront);
				_front = newFront;
				--_size;
				if (_size == 0)
				{
					_back = nullptr;
				}
			}
		}

		void clear() noexcept
		{
			if (0 < _size)
			{
				node* curr = _front;
				while (curr != nullptr)
				{
					node* next = curr->_next;
					SafeDelete(curr);
					curr = next;
				}
				_size = 0;
				_front = nullptr;
				_back = nullptr;
			}	
		}

		template<typename VT>
		void push_back(VT&& value)
		{

		}

		void pop_back()
		{

		}

	private:
		size_t _size = 0;
		node* _front = nullptr;
		node* _back = nullptr;


		void link_node(node* lhs, node* rhs)
		{
			lhs->_next = rhs;
			rhs->_prev = lhs;
			if (_front == rhs) _front = lhs;	
			else if (_back == lhs) _back = rhs;
		}
	};

}