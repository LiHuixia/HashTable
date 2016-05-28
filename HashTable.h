#pragma once
#include<iostream>
#include<string>

namespace First
{
	enum State
	{
		EMPTY,
		DELETE,
		EXITS,
	};

	template<class K>
	class HashTable
	{
	public:
		HashTable(size_t capacity = 10)
			:_table(new K[capacity])
			, _state(new State[capacity])
			, _size(0)
			, _capacity(capacity)
		{
			for (size_t i = 0; i < capacity; ++i)
			{
				_state[i] = EMPTY; //memset(_state,EMPTY,sizeof(State)*_capacity)(错误，以单个字节初始化)
			}
		}

		~HashTable()
		{
			if (_table != NULL)
			{
				delete[] _table;
				delete[] _state;
			}
		}

		HashTable(const HashTable<K> & h)
			:_table(NULL)
			, _state(NULL)
			, _size(0)
			, _capacity(0)

		{
			HashTable<K> tmp(h._capacity);
			for (size_t i = 0; i < tmp._capacity; ++i)
			{
				if (h._state[i] == EXITS)
				{
					tmp.Insert(h._table[i]);
				}
			}
			swap(tmp._table, _table);
			swap(tmp._state, _state);
			swap(tmp._capacity, _capacity);
			swap(tmp._size, _size);
				
		}

		HashTable<K>& operator=(HashTable<K> ha)
		{
			swap(_table, ha._table);
			swap(_state, ha._state);
			swap(_size, ha._size);
			swap(_capacity, ha._capacity);
			return *this;
		}
		bool Insert(const K& key)
		{
			_check();
			size_t index = _HashFunc(key); //位置

			
			//线性探索
			while (_state[index] == EXITS)  //若位置已存在数据，进行处理
			{
				if (_table[index] == key)
				{
					return false;
				}
				++index;
				if (index == _capacity)
				{
					index = 0;
				}
			}
			_table[index] = key;    //没有，直接插入
			_state[index] = EXITS;
			++_size;
			
		}

		bool Find(const K& key)
		{
			size_t index = _HashFunc(key);
			size_t start = index;
			while (_state[index] != EMPTY)   //为存在或已删除，寻找
			{
				if (_table[index] == key)    
				{
					if (_state[index] == DELETE)    //找到了，但是为删除
					{
						return false;
					}
					else
					{
						return true;
					}
				}
				++index;
				if (index == _capacity)   
				{
					index = 0;
				}
				if (index == start)   //找了一圈还没找到
				{
					return false;
				}
			}
			return false;
		}

		bool Remove(const K& key)
		{
			size_t index = _HashFunc(key);
			size_t start = index;
			while (_state[index] == EXITS)  //只为存在，则删除
			{
				if (_table[index] == key)
				{
					_state[index] = DELETE;
					_size--;
					return true;
				}
				++index;
				if (index == _capacity)
				{
					index = 0;
				}
				if (start == index)
				{
					return false;
				}
			}
			return false;
		}
		void Print()
		{
			for (size_t i = 0; i < _capacity; ++i)
			{
				
				cout << "[" << _state[i] << "," << _table[i] << "]" << endl;
			}
			cout << "---------------------------------" << endl;
		}
	protected:
		void _check()
		{
			if (_size * 10 / _capacity == 7)
			{
				HashTable<K> tmp(2 * _capacity);
				for (size_t i = 0; i < _capacity; i++)
				{
					if (_state[i] == EXITS)
					{
						tmp.Insert(_table[i]);
					}
				}
				swap(tmp._table, _table);
				swap(tmp._state, _state);
				swap(tmp._capacity, _capacity);
				swap(tmp._size, _size);
			}
		}
		size_t _HashFunc(const K& key)
		{
			return key%_capacity;
		}
	protected:
		K *_table;
		State * _state;
		size_t _size; //表中元素个数
		size_t _capacity;
	};

	void testHashTable()
	{
		HashTable<int> ha;

		ha.Insert(89);
		ha.Insert(18);
		ha.Insert(49);
		ha.Insert(58);
		ha.Insert(9);
		/*ha.Print();
		printf("%d\n", ha.Find(1));
		printf("%d\n", ha.Find(9));
		printf("%d\n", ha.Find(10));
		printf("------------------\n");
		printf("%d\n", ha.Remove(1));
		ha.Remove(9);
		ha.Print();*/
		HashTable<int> ha1(ha);
		ha1.Print();
		HashTable<int> ha2;
		ha2 = ha1;
		ha2.Print();
	}
}


namespace Second
{
	enum State
	{
		EMPTY,
		DELETE,
		EXITS,
	};

	template<class K,class V>
	struct HashTableNode
	{
		K _key;
		V _value;
		HashTableNode()
		{}
		HashTableNode(const K& key , const V& value )
			:_key(key)
			, _value(value)
		{}
	};

	template<class K>
	struct _HashFunc
	{
		size_t operator()(const K& key)
		{
			return key;
		}
	};

	template<>
	struct _HashFunc<std::string>
	{
		size_t operator()(const std::string& str)
		{
			size_t sum = 0;
			for (size_t i = 0; i < str.size(); i++)
			{
				sum += str[i];
			}
			return sum;
		}
	};

	template<class K, class V, class HashModel = _HashFunc<K>>
	class HashTable
	{
	public:
		typedef HashTableNode<K, V> Node;

	public:
		HashTable(size_t capacity = 10)
			:_table(new Node[capacity])
			, _state(new State[capacity])
			, _size(0)
			, _capacity(capacity)
		{
			for (size_t i = 0; i < capacity; i++)
			{
				_state[i] = EMPTY;
			}
		}

		~HashTable()
		{
				if (_table != NULL)
				{
					delete[] _table;
					delete[] _state;
					_size = 0;
					_capacity = 0;
				}
		}
		bool Insert(const K& key, const V& value)
		{
			_check();
			size_t start = _HashFunc(key);
			size_t index = start;
			size_t i = 1;
				//二次探测
			while (_state[index] == EXITS)
			{
				if (_table[index]._key == key)
				{
					return false;
				}
				index = start + i*i;
				i++;
				if (index = _capacity)
				{
					index = 0;
				}
				if (index == start)
				{
					return false;
				}
			}
			_table[index] = Node(key, value);
			
			_state[index] = EXITS;
			++_size;
			return true;
		}	

		Node* Find(const K& key)
		{
			size_t index = _HashFunc(key);
			size_t start = index;
			while (_state[index] != NULL)
			{
				if (_table[index]._key == key)
				{
					if (_state[index] == DELETE)
					{
						return NULL;
					}
					else
					{
						return &_table[index];
					}
				}
				++index;
				if (index == _capacity)
				{
					index = 0;
				}
				if (index = start)
				{
					return NULL;
				}
			}
		}
		void Print()

		{
			for (int i = 0; i < _capacity; ++i)
			{
				printf("[ %d ,", _table[i]._key);
				printf("%d", _table[i]._value);
				printf("]--- %d\n", _state[i]);
			}
			cout << endl;
		}
	protected:
		void _check()
		{
			if (_size * 10 / _capacity == 7)
			{
				HashTable<K, V, HashModel > tmp(2 * _capacity);
				for (size_t i = 0; i < _capacity; ++i)
				{
					if (_state[i] == EXITS)
					{
						tmp.Insert(_table[i]._key, _table[i]._value);
					}
				}
				swap(tmp._table, _table);
				swap(tmp._state, _state);
				swap(tmp._capacity, _capacity);
				swap(tmp._size, _size);
			}
		}
		size_t _HashFunc(const K& key)
		{
			return key%_capacity;
		}
	protected:
		Node* _table;
		State* _state;
		size_t _size;
		size_t _capacity;
	/*	HashModel _HF;*/
	};


	void testFun()
	{
		HashTable<int, int> str;
		str.Insert(1, 1);
		str.Insert(2, 2);
		str.Insert(3, 5);
		str.Print();

		HashTableNode<int, int> *ret = str.Find(3);
		if (ret != NULL)
		{
			std::cout << ret->_value << std::endl;
		}
	
	}
}

