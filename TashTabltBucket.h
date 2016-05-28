#pragma once
#include<iostream>
#include<vector>
using namespace std;

template<class K, class V>
struct HashNode
{
	K _key;
	V _value;
	HashNode<K, V>* _next;
	HashNode(const K& key, const K& value)
		:_key(key)
		, _value(value)
		, _next(NULL)
	{}
};

template<class K, class V>
class HashTable
{
	typedef HashNode<K, V> Node;
public:
	HashTable(size_t capacity)
		:_size(0)
	{
		_table.resize(capacity);
	}

	~HashTable()
	{
		if (_table.size()!=0)
		{
			for (size_t i = 0; i < _table.size(); ++i)
			{
				Node *cur = _table[i];
				while (cur)
				{
					Node *del = cur;
					cur = cur->_next;
					delete del;
					del = NULL;
				}
			}
		}
	}
	bool Insert(const K& key, const V& value)
	{
		_check();
		size_t index = HashFunc(key);
		Node *cur = _table[index];
		while (cur)
		{
			if (cur->_key == key)  //������
			{
				return false;
			}
			cur = cur->_next;
		}
		Node *tmp = new Node(key, value);//ͷ��
		tmp->_next = _table[index];
		_table[index] = tmp;
		++_size;
		return true;
	}

	void Print()
	{
		for (size_t i = 0; i < _table.size(); ++i)
		{
			Node *cur = _table[i];
			while (cur)
			{
				cout << "[" << cur->_key << "," << cur->_value << "]" << "--->";
				cur = cur->_next;
			}
			cout << "NULL" << endl;
		}
		cout << endl;
	}
	Node * Find(const K& key)
	{
		size_t index = HashFunc(key);
		Node *cur = _table[index];
		while (cur)
		{
			if (cur->_key == key)
			{
				return cur;
			}
			cur = cur->_next;
		}
		return NULL;
	}

	bool Remove(const K& key)
	{
		size_t index = HashFunc(key);
		Node* cur = _table[index];
		if (cur == NULL)
		{
			return false;
		}
		if (cur->_key == key)
		{
			_table[index] = cur->_next;
			delete cur;
			cur = NULL;
			--_size;
			return true;
		}
		Node *prev = cur;
		cur = cur->_next;
		while (cur)
		{
			if (cur->_key = key)
			{
				prev->_next = cur->_next;
				delete cur;
				cur = NULL;
				--_size;
				return true;
			}
			prev = cur;
			cur = cur->_next;
		}
		return false;
	}
protected:
	size_t NewSize()   //������������������
	{
		const int _PrimeSize = 28;
		static const unsigned long _PrimeList[_PrimeSize] =
		{
			53ul, 97ul, 193ul, 389ul, 769ul,
			1543ul, 3079ul, 6151ul, 12289ul, 24593ul,
			49157ul, 98317ul, 196613ul, 393241ul, 786433ul,
			1572869ul, 3145739ul, 6291469ul, 12582917ul, 25165843ul,
			50331653ul, 100663319ul, 201326611ul, 402653189ul, 805306457ul,
			1610612741ul, 3221225473ul, 4294967291ul
		};
		for (size_t i = 0; i < _PrimeSize; i++)
		{
			if (_PrimeList[i]>_table.size())
			{
				return _PrimeList[i];
			}
		}
		return _PrimeList[_PrimeSize - 1];   //�����Ľ�������
	}

	void _check()
	{
		if (_size == _table.size())
		{
			size_t NextPrime = NewSize();
			vector<Node*> NewTable;
			NewTable.reserve(NextPrime);
			for (size_t i = 0; i < _table.size(); ++i)//��ԭ����ϣͰ�е�Ԫ��ժ���������뵽�µĹ�ϣͰ
			{
				Node* cur = _table[i];
				while (cur)       
				{
					Node* tmp = cur;
					cur = cur->_next;
					size_t index = HashFunc(tmp->_key);  
					tmp->_next = NewTable[index];
					NewTable[index] = tmp;
				}
				_table[i] = NULL;    //��ԭλ���ÿ�
			}
			_table.swap(NewTable);		
		}
	}
	size_t HashFunc(const K& key)
	{
		return key % (_table.size());
	}
protected:
	vector <Node*> _table;
	size_t _size;
	//Node *_table[]; //ָ�����飬���ڴ洢ָ�������
	//size_t _size;
	//size_t _capacity;
};

void Test()
{
	HashTable<int, int> ha(10);
	ha.Insert(1, 1);
	ha.Insert(2, 2);
	ha.Insert(11, 2);
	ha.Insert(20, 1);
	ha.Insert(33, 1);
	ha.Insert(55, 2);
	ha.Print();
	/*cout<<ha.Find(1)->_value<<endl;
	cout<<ha.Find(55)->_value<<endl;*/
	ha.Remove(1);
	ha.Remove(11);
	ha.Print();

}
