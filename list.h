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

		//����ڵ�Ĺ��캯��     T()��ʾһ����������
		__list_node(const T& x = T())
			:_date(x)
			, _next(nullptr)
			, _prev(nullptr)
		{}
	};

	//����������Ҳֻ��һ����Ա����������ָ���Ǹ�����ָ��
	template<class T,class Ref,class Ptr>
	struct __list_iterator
	{
		typedef __list_node<T> node;
		typedef __list_iterator<T,Ref,Ptr> self;
		node* _node;

		__list_iterator(node* node)
			:_node(node)
		{}
		//�������� ��ֵ����  ���������ȶ�����Ҫȥд��Ĭ�����ɵľ���

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

		//ǰ��++
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
		typedef __list_iterator<T, const T&, const T*> const_iterator;//�������ı��ʾ��ǣ��ڲ��ƻ���ķ�װ������£����ܹ����ʺ��޸���
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


		
		//__list_node<T>���������ͣ����ǹ��캯������������ͬ
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
			_head = nullptr; //��ʵ���������������ڶ����ˣ�û���˿����õ������������������������п��п���
		}

		void clear()
		{
			iterator it = begin();
			while (it != end())
			{
				it = earse(it); //���ص�ɾ��λ�õ���һ��
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

			//�ִ�д��
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


		//���ÿ��Ժܺý�����ι����е��������
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

		//���������һ��λ�ò��붼��ǰһ���ͺ�һ��
		//��������������λ�ò���  
		//���ڵ�������˵��ǰ������ݸı��ˣ��ϸ���������˵����ʵҲ�������ʧЧ��
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
			
			//���ص����²����Ǹ����ĵ�����
			return iterator(newnode);
		}

		//ɾ���ɲ���ɾ��_head����Ϊɾ����ṹֱ�Ӿͻ��������
		iterator earse(iterator pos)
		{
			assert(pos != end()); //��Ϊ��������һ������ҿ��Ľṹ��begin()�����ڵ�һ������λ�ã�end()������_head��λ��
			//ֻ��Ҫ�ҵ�����ǰһ���ͺ�һ��
			node* cur = pos._node;
			node* prev = cur->_prev;
			node* next = cur->_next;

			prev->_next = next;
			next->_prev = prev;

			delete cur; //���λ�ö���earse�ˣ��㻹ȥ++������Ұָ��ķ�����
			return iterator(next);
		}
	private:
		//˫���ͷѭ������
		node* _head;
	};
}