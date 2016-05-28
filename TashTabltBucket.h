#pragma once

namespace BUCKET
{
	template<class K,class V>
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
}