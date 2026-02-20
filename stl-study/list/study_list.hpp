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
		template <bool CONST_ITER>
		class iterator_template
		{
			using iter_type = iterator_template<CONST_ITER>;
			using iterator = iterator_template<false>;
			using const_iterator = iterator_template<true>;
			using iter_element_type = std::conditional_t<CONST_ITER, const element_type, element_type>;
			using iter_node_dummy_type = std::conditional_t<CONST_ITER, const dummy_node, dummy_node>;
			using iter_node_type = std::conditional_t<CONST_ITER, const node, node>;
			friend class container_type;

		public:
			iterator_template(iter_node_dummy_type* nd) : _currentNode(nd) {}
			iterator_template(const iter_type& rhs) = default;
			~iterator_template() = default;

			template <bool rhsConst> 
			iterator_template(const iterator_template<rhsConst>& rhs) requires (CONST_ITER && !rhsConst) : _currentNode(rhs._currentNode) {}

			iter_element_type& get() const
			{
				return get_node()->_data;
			}

			iter_element_type& operator*() const noexcept
			{
				return get_node()->_data;
			}

			iter_element_type* operator->() const noexcept
			{
				return &get_node()->_data;
			}

			iter_type operator++()
			{
				if (_currentNode->_next == nullptr) throw std::runtime_error("next iterator is null");
				else _currentNode = _currentNode->_next;
				return *this;
			}

			iter_type operator++(int)
			{
				iter_type temp = *this;
				if (_currentNode->_next == nullptr) throw std::runtime_error("next iterator is null");
				else _currentNode = _currentNode->_next;
				return temp;
			}

			iter_type operator--()
			{
				if (_currentNode->_prev == nullptr) throw std::runtime_error("prev iterator is null");
				else _currentNode = _currentNode->_prev;
				return *this;
			}

			iter_type operator--(int)
			{
				iter_type temp = *this;
				if (_currentNode->_prev == nullptr) throw std::runtime_error("prev iterator is null");
				else _currentNode = _currentNode->_prev;
				return temp;
			}

			bool operator==(const iterator& rhs) const noexcept
			{
				return _currentNode == rhs._currentNode;
			}

			bool operator==(const const_iterator& rhs) const noexcept
			{
				return _currentNode == rhs._currentNode;
			}

			bool operator==(std::nullptr_t nptr) const noexcept
			{
				return _currentNode == nptr;
			}

		private:
			iter_node_dummy_type* _currentNode = nullptr;
			iter_node_type* get_node() const
			{
				return static_cast<iter_node_type*>(_currentNode);
			}
		};
		using iterator = iterator_template<false>;
		using const_iterator = iterator_template<true>;

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

		const_iterator cbegin() const
		{
			return const_iterator(get_front_node());
		}

		const_iterator cend() const
		{
			return const_iterator(&_backDummy);
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