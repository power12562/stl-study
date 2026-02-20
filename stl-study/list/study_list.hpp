#include "common/memory.h"

namespace mst
{
	template <typename T>
	class list
	{
		using element_type = T;
		using container_type = list<element_type>;

		struct node;
		struct dummy_node
		{
			node* _prev = nullptr;
			node* _next = nullptr;
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
			iterator(dummy_node* nd) : _currentNode(static_cast<node*>(nd)) {}
			iterator(node* nd) : _currentNode(nd) {}
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
			node* front = get_front_node();
			if (front == nullptr) throw std::runtime_error("front node is null.");
			else
			{
				return iterator(front);
			}
		}

		iterator end()
		{
			node* back = get_back_node();
			if (back == nullptr) throw std::runtime_error("back node is null.");
			else
			{
				return iterator(&_backDummy);
			}
		}

		element_type& front()
		{
			node* front = get_front_node();
			if (front == nullptr) throw std::runtime_error("front node is null.");
			else return front->_data;
		}

		const element_type& front() const
		{
			const node* front = get_front_node();
			if (front == nullptr) throw std::runtime_error("front node is null.");
			else return front->_data;
		}

		element_type& back()
		{
			node* back = get_back_node();
			if (back == nullptr) throw std::runtime_error("back node is null.");
			else return back->_data;
		}

		const element_type& back() const
		{
			const node* back = get_back_node();
			if (back == nullptr) throw std::runtime_error("back node is null.");
			else return back->_data;
		}

		template<typename VT>
		void push_front(VT&& value)
		{
			node* prevFront = get_front_node();
			node* newFront = new node(std::forward<VT>(value));
			if (0 < _size)
			{
				link_node(newFront, prevFront);
			}
			else
			{
				set_front_node(newFront);
				set_back_node(newFront);
			}
			++_size;
		}

		void pop_front() noexcept
		{
			node* prevFront = get_front_node();
			if (prevFront != nullptr)
			{
				node* newFront = prevFront->_next;
				if (newFront != nullptr)
				{
					newFront->_prev = nullptr;
				}
				SafeDelete(prevFront);
				set_front_node(newFront);
				if (_size == 1) set_back_node(nullptr);
				--_size;
			}
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
				_size = 0;
				set_front_node(nullptr);
				set_back_node(nullptr);
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

		node* get_front_node()
		{
			return _frontDummy._next;
		}

		const node* get_front_node() const
		{
			return _frontDummy._next;
		}

		void set_front_node(node* nd)
		{
			_frontDummy._next = nd;
			if (nd != nullptr) nd->_prev = static_cast<node*>(&_frontDummy);	
		}

		node* get_back_node()
		{
			return _backDummy._prev;
		}

		const node* get_back_node() const
		{
			return _backDummy._prev;
		}

		void set_back_node(node* nd)
		{
			_backDummy._prev = nd;
			if (nd != nullptr) nd->_next = static_cast<node*>(&_backDummy);
		}

		void link_node(node* lhs, node* rhs)
		{
			node* front = get_front_node();
			node* back = get_back_node();
			lhs->_next = rhs;
			rhs->_prev = lhs;
			if (front == rhs) set_front_node(lhs);
			else if (back == lhs) set_back_node(rhs);
		}
	};

}