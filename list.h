#pragma once
#include<assert.h>
namespace wzy
{
	template<class T>
	struct __list_node
	{
		T _date;
		__list_node<T>* _next;
		__list_node<T>* _prev;

		//这个节点的构造函数     T()表示一个匿名对象
		__list_node(const T& x = T())
			:_date(x)
			, _next(nullptr)
			, _prev(nullptr)
		{}
	};

	//迭代器里面也只有一个成员变量，就是指向那个结点的指针
	template<class T,class Ref,class Ptr>
	struct __list_iterator
	{
		typedef __list_node<T> node;
		typedef __list_iterator<T,Ref,Ptr> self;
		node* _node;

		__list_iterator(node* node)
			:_node(node)
		{}
		//拷贝构造 赋值重载  析构函数等都不需要去写，默认生成的就行

		bool operator!=(const self& s)
		{
			return _node != s._node;
		}

		bool operator==(const self& s)
		{
			return !(*this != s);
		}

		Ref operator*()const
		{
			return _node->_date;
		}

		//前置++
		self& operator++()
		{
			_node = _node->_next;
			return *this;
		}

		self operator++(int)
		{
			self tmp(*this);
			_node = _node->_next;
			return tmp;
		}


		self& operator--()
		{
			_node = _node->_prev;
			return *this;
		}

		self operator--(int)
		{
			self tmp(*this);
			_node = _node->_prev;
			return tmp;
		}

		Ptr operator->()const
		{
			return &_node->_date;
		}
	};

	template<class T>
	class list
	{
		/*typedef __list_node<T> node;*/
	public:
		typedef __list_node<T> node;
		typedef __list_iterator<T, T&, T*> iterator;
		typedef __list_iterator<T, const T&, const T*> const_iterator;//迭代器的本质就是，在不破坏你的封装的情况下，还能够访问和修改你
		iterator begin()
		{
			return _head->_next;
		}

		iterator end()
		{
			return _head;
		}

		const_iterator begin()const
		{
			return _head->_next;
		}

		const_iterator end()const
		{
			return _head;
		}


		
		//__list_node<T>是他的类型，但是构造函数是与类名相同
		list()
		{
			_head = new node;
			_head->_next = _head;
			_head->_prev = _head;
		}


		~list()
		{
			clear();
			delete _head;
			_head = nullptr; //其实这个对象的声明周期都到了，没有人可以拿到，所以这句代码在析构函数中可有可无
		}

		void clear()
		{
			iterator it = begin();
			while (it != end())
			{
				it = earse(it); //返回的删除位置的下一个
			}
		}

		//lt2(lt1)
		list(const list<T>& lt)
		{
			//_head = new node;
			//_head->_next = _head;
			//_head->_prev = _head;


			//const_iterator it = lt.begin();
			//while (it != lt.end())
			//{
			//	push_back(*it);
			//	++it;
			//}

			//现代写法
			_head = new node;
			_head->_next = _head;
			_head->_prev = _head;

			list<T> tmp(lt.begin(), lt.end());
			std::swap(_head, tmp._head); 
		}


		list<T>& operator=(list<T> lt)
		{
			std::swap(_head, lt._head);
			return *this;
		}



		template<class InputIterator>
		list(InputIterator first, InputIterator last)
		{
			_head = new node;
			_head->_next = _head;
			_head->_prev = _head;

			while (first != last)
			{
				push_back(*first);
				++first;
			}
		}


		//引用可以很好解决传参过程中的深拷贝问题
		void push_back(const T& x)
		{
			//node* newnode = new node(x);
			//node* tail = _head->_prev;
			////_head  tail  newnode
			//tail->_next = newnode;
			//newnode->_prev = tail;
			//newnode->_next = _head;
			//_head->_prev = newnode;

			insert(end(), x);
		}

		void push_front(const T& x)
		{
			insert(begin(), x);
		}

		void pop_back()
		{
			earse(--end());
		}

		void pop_front()
		{
			earse(begin());
		}

		//你在任意的一个位置插入都是前一个和后一个
		//插入可以在任意的位置插入  
		//对于迭代器来说，前后的数据改变了，严格意义上来说，其实也算迭代器失效了
		iterator insert(iterator pos, const T& x)
		{
			node* newnode = new node(x);
			node* cur = pos._node;
			node* prev = cur->_prev;

			//prev newnode cur
			prev->_next = newnode;
			newnode->_prev = prev;
			newnode->_next = cur;
			cur->_prev = newnode;
			
			//返回的是新插入那个结点的迭代器
			return iterator(newnode);
		}

		//删除可不敢删除_head，因为删掉大结构直接就会出现问题
		iterator earse(iterator pos)
		{
			assert(pos != end()); //因为迭代器是一个左闭右开的结构，begin()就是在第一个结点的位置，end()就是在_head的位置
			//只需要找到它的前一个和后一个
			node* cur = pos._node;
			node* prev = cur->_prev;
			node* next = cur->_next;

			prev->_next = next;
			next->_prev = prev;

			delete cur; //这个位置都被earse了，你还去++，就是野指针的访问了
			return iterator(next);
		}
	private:
		//双向带头循环链表
		node* _head;
	};
}