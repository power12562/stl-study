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
			iterator_template() = default;
			iterator_template(iter_node_dummy_type* nd) : _currentNode(nd) {}
			iterator_template(const iter_type& rhs) = default;
			~iterator_template() = default;

			template <bool rhsConst> 
			iterator_template(const iterator_template<rhsConst>& rhs) requires (CONST_ITER && !rhsConst) : _currentNode(rhs._currentNode) {}

			iter_element_type& get() const
			{
				return get_node()->_data;
			}

			iter_type& operator=(iter_type rhs)
			{
				swap(*this, rhs);
				return *this;
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

			template <bool rhsConst> bool operator==(const iterator_template<rhsConst>& rhs) const
			{
				return _currentNode == rhs._currentNode;
			}

		private:
			iter_node_dummy_type* _currentNode = nullptr;
			iter_node_type* get_node() const
			{
				return static_cast<iter_node_type*>(_currentNode);
			}

			static void swap(iter_type& lhs, iter_type& rhs)
			{
				std::swap(lhs._currentNode, rhs._currentNode);
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

		iterator insert(iterator pos, const element_type& value)
		{
			if (pos._currentNode == nullptr) throw std::runtime_error("Invalid iterator.");
			else
			{
				node* newNode = new node(value);
				dummy_node* rightNode = pos._currentNode;
				dummy_node* leftNode = rightNode->_prev;
				link_node(leftNode, newNode, rightNode);
				++_size;
			}
		}

		iterator insert(iterator pos, element_type&& value)
		{
			if (pos._currentNode == nullptr) throw std::runtime_error("Invalid iterator.");
			
			node* newNode = new node(std::move(value));
			dummy_node* rightNode = pos._currentNode;
			dummy_node* leftNode = rightNode->_prev;
			link_node(leftNode, newNode, rightNode);
			++_size;

			return iterator(newNode);
		}

		void erase(iterator pos)
		{
			if (pos._currentNode == nullptr) throw std::runtime_error("Invalid iterator.");
			dummy_node* currentNode = pos._currentNode;
			if (currentNode == &_frontDummy) throw std::runtime_error("Invalid iterator.");
			if (currentNode == &_backDummy) throw std::runtime_error("Invalid iterator.");

			node* eraseNode = static_cast<node*>(currentNode);
			dummy_node* lNode = eraseNode->_prev;
			dummy_node* rNode = eraseNode->_next;
			link_node(lNode, rNode);
			SafeDelete(eraseNode);
			--_size;
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
			if (empty()) throw std::runtime_error("list is empty.");
			else  
			{
				iterator front = get_front_node();
				return *front;
			}
		}

		const element_type& front() const
		{
			if (empty()) throw std::runtime_error("list is empty.");
			else
			{
				const_iterator front = static_cast<const node*>(get_front_node());
				return *front;
			}
		}

		element_type& back()
		{		
			if (empty()) throw std::runtime_error("list is empty.");
			else
			{
				iterator back = get_back_node();
				return *back;
			}
		}

		const element_type& back() const
		{	
			if (empty()) throw std::runtime_error("list is empty.");
			else
			{
				const_iterator back = static_cast<const node*>(get_back_node());
				return *back;
			}	
		}

		template<typename VT>
		void push_front(VT&& value)
		{
			iterator front = begin();
			insert(front, std::forward<VT>(value));
		}

		void pop_front() noexcept
		{
			if (empty()) return;

			iterator front = begin();
			erase(front);
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