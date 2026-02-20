#include "common/memory.h"

namespace mst
{
	template <typename T>
	class list
	{
		using element_type = T;
		using container_type = list<element_type>;

		struct dummy_node
		{
			dummy_node* _prev = nullptr;
			dummy_node* _next = nullptr;
		};

		struct node : public dummy_node
		{
			node(const element_type& data) : _data(data) {}
			element_type _data;
		};

	public:

		class iterator
		{
			friend class container_type;
		public:
			iterator(dummy_node* nd) : _currentNode(nd) {}
			iterator(const iterator& rhs) : _currentNode(rhs._currentNode) {}
			~iterator() = default;

			element_type& get()
			{
				return get_node()->_data;
			}

			element_type& operator*() noexcept
			{
				return get_node()->_data;
			}

			element_type* operator->() noexcept
			{
				return &get_node()->_data;
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
				if (_currentNode->_prev == nullptr) throw std::runtime_error("prev iterator is null");
				else _currentNode = _currentNode->_prev;
				return *this;
			}

			iterator operator--(int)
			{
				iterator temp = *this;
				if (_currentNode->_prev == nullptr) throw std::runtime_error("prev iterator is null");
				else _currentNode = _currentNode->_prev;
				return temp;
			}

			bool operator==(const iterator& rhs) const noexcept
			{
				return _currentNode == rhs._currentNode;
			}

			bool operator==(std::nullptr_t nptr) const noexcept
			{
				return _currentNode == nptr;
			}

		private:
			dummy_node* _currentNode = nullptr;
			node* get_node()
			{
				return static_cast<node*>(_currentNode);
			}
		};

		list()
		{
			reset_field();
		}
		~list()
		{
			clear();
		}

		size_t size() const
		{
			return _size;
		}

		bool empty() const
		{
			return _size == 0;
		}

		iterator begin()
		{
			return iterator(get_front_node());
		}

		iterator end()
		{
			return iterator(&_backDummy);
		}

		element_type& front()
		{
			iterator front = get_front_node();
			if (front == nullptr) throw std::runtime_error("front node is null.");
			else return *front;
		}

		const element_type& front() const
		{
			const node* front = static_cast<const node*>(get_front_node());
			if (front == nullptr) throw std::runtime_error("front node is null.");
			else return front->_data;
		}

		element_type& back()
		{
			iterator back = get_back_node();
			if (back == nullptr) throw std::runtime_error("back node is null.");
			else return *back;
		}

		const element_type& back() const
		{
			const node* back = static_cast<const node*>(get_back_node());
			if (back == nullptr) throw std::runtime_error("back node is null.");
			else return back->_data;
		}

		template<typename VT>
		void push_front(VT&& value)
		{
			dummy_node* dummy = &_frontDummy;
			dummy_node* prevFront = get_front_node();
			node* newFront = new node(std::forward<VT>(value));
			link_node(dummy, newFront, prevFront);
			++_size;
		}

		void pop_front() noexcept
		{
			if (empty()) return;

			node* prevFront = static_cast<node*>(get_front_node());
			dummy_node* newFront = prevFront->_next;
			link_node(prevFront->_prev, newFront);
			SafeDelete(prevFront);
			--_size;
		}

		void clear() noexcept
		{
			if (0 < _size)
			{
				iterator iter = begin();
				iterator endIter = end();
				while (iter != endIter)
				{
					iterator curr = iter;
					++iter;
					SafeDelete(curr._currentNode);
				}
				reset_field();
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
		dummy_node _frontDummy;
		dummy_node _backDummy;

		void reset_field()
		{
			_size = 0;
			_frontDummy._next = &_backDummy;
			_backDummy._prev = &_frontDummy;
		}

		dummy_node* get_front_node()
		{
			return _frontDummy._next;
		}

		const dummy_node* get_front_node() const
		{
			return _frontDummy._next;
		}

		dummy_node* get_back_node()
		{
			return _backDummy._prev;
		}

		const dummy_node* get_back_node() const
		{
			return _backDummy._prev;
		}

		void link_node(dummy_node* lhs, dummy_node* mhs, dummy_node* rhs)
		{
			lhs->_next = mhs;
			mhs->_prev = lhs;
			mhs->_next = rhs;
			rhs->_prev = mhs;
		}

		void link_node(dummy_node* lhs, dummy_node* rhs)
		{
			rhs->_prev = lhs;
			lhs->_next = rhs;
		}
	};

}