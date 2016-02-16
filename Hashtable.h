#ifndef HASHTABLE_INCLUDED
#define HASHTABLE_INCLUDED

#include <string>

template <typename KeyType, typename ValueType>
class HashTable
{
public:
	HashTable(unsigned int numBuckets, unsigned int capacity);
	~HashTable();
	bool isFull()	const;
	bool set(const KeyType&	key, const ValueType& value, bool permanent = false);
	bool get(const KeyType& key, ValueType& value)	const;
	bool touch(const KeyType& key);
	bool discard(KeyType& key, ValueType& value);
private:
	//	We	prevent a	HashTable from	being	copied	or	assigned	by	declaring	the
	//	copy	constructor	and	assignment	operator	private	and	not	implementing	them.
	struct Node
	{
		Node* m_next;
		KeyType m_key;
		ValueType m_value;
	};
	struct Node1
	{
		Node1* m_next;
		Node1* m_prev;
		KeyType m_key;
		ValueType m_value;
	};
	unsigned int m_capacity;
	unsigned int m_numBuckets;
	unsigned int m_size;
	Node1* m_listHead;
	Node1* m_listTail;
	Node** m_table;
	HashTable(const HashTable&);
	HashTable& operator=(const HashTable&);
	Node* isPresent(const KeyType& key) const;
	unsigned int getBucketVal(const KeyType& key) const;
	void udpdateAssociation(const KeyType& key, const ValueType& value);
	void insertInFront(const KeyType& key, const ValueType& value, Node* &begin);
	void insertInFront1(const KeyType& key, const ValueType& value, Node1* &begin);
	unsigned int computeBucket(const unsigned int& hash) const;
};

template <typename KeyType, typename ValueType>
HashTable<KeyType, ValueType>::HashTable(unsigned int numBuckets, unsigned int capacity)		//Constructor
	:m_capacity(capacity), m_size(0), m_numBuckets(numBuckets)
{
	m_table = new Node*[numBuckets];
	m_listHead = nullptr;
	m_listTail = nullptr;
	for (unsigned int i = 0; i < numBuckets; i++)
	{
		m_table[i] = nullptr;
	}
}

template <typename KeyType, typename ValueType>			//Destructor
HashTable<KeyType, ValueType>::~HashTable()	
{
	for (int i = 0; i < m_numBuckets; i++)		//Deallocate everything in hash table
	{
		Node* p = m_table[i];
		while (p != nullptr)
		{
			Node* q = p->m_next;
			delete p;
			p = q;
		}
	}
	while (m_listHead != m_listTail)		//Deallocate everything in association tracking list
	{
		Node1* p = m_listHead;
		m_listHead = p->m_next;
		p->m_next->m_prev = nullptr;
		delete p;
	}
	m_listTail = nullptr;
	delete m_listHead;
	m_listHead = nullptr;
}

template <typename KeyType, typename ValueType>
unsigned int HashTable<KeyType, ValueType>::computeBucket(const unsigned int& hash) const
{
	return hash % m_numBuckets;
}

template <typename KeyType, typename ValueType>
bool HashTable<KeyType, ValueType>::isFull() const
{
	if (m_size >= m_capacity)
		return true;
	else
		return false;
}

template <typename KeyType, typename ValueType>
bool HashTable<KeyType, ValueType>::touch(const KeyType& key)
{
	Node1* p = m_listHead;
	while (p != nullptr && p->m_key != key)			//Find the key in the association tracking list
	{
		p = p->m_next;
	}

	if (p != nullptr)
	{
		if (p != m_listHead)
		{
			Node1* q = p->m_prev;						//Deallocate that node after adjusting pointers
			q->m_next = p->m_next;
			if (p != m_listTail)
			{
				p->m_next->m_prev = q;
			}
			else
				m_listTail = q;
			insertInFront1(key, p->m_value, m_listHead);		//Insert New node with same key and value in the front of the list
			delete p;
		}
		return true;
	}
	return false;
}

template <typename KeyType, typename ValueType>
bool HashTable<KeyType, ValueType>::set(const KeyType& key, const ValueType& value, bool permanent)
{
		Node* p = isPresent(key);
		if (p == nullptr)
		{
			if (isFull())
				return false;
			m_size++;
			unsigned int bucket = getBucketVal(key);			//If not already present, Insert to front of list associated with required bucket
			insertInFront(key, value, m_table[bucket]);		
		}		
		else
		{
			p->m_value = value;			//Otherwise update value
		}

		if (permanent != true)
		{
			udpdateAssociation(key, value);			//Update association tracking table
		}
		return true;
}

template <typename KeyType, typename ValueType>
typename HashTable<KeyType, ValueType>::Node* HashTable<KeyType, ValueType>::isPresent(const KeyType& key) const
{
	unsigned int bucket = getBucketVal(key);
	Node* p = m_table[bucket];
	while (p != nullptr)		//If Node of given key is present in the table, return poninter to its node
	{
		if (p->m_key == key)
			return p; 
		else
			p = p->m_next;
	}
	return nullptr;
}

template <typename KeyType, typename ValueType>
bool HashTable<KeyType, ValueType>::get(const KeyType& key, ValueType& value) const
{
	Node* p = isPresent(key);
	if (p == nullptr)			//If the key exists in the table, set value to its value
		return false;
	else
	{
		value = p->m_value;
		return true;
	}
}

template <typename KeyType, typename ValueType>
unsigned int HashTable<KeyType, ValueType>::getBucketVal(const KeyType& key) const
{
	unsigned int computeHash(KeyType);
	unsigned int hash = computeHash(key);
	unsigned int bucket = computeBucket(hash);
	return bucket;
}

template <typename KeyType, typename ValueType>
void HashTable<KeyType, ValueType>::insertInFront(const KeyType& key, const ValueType& value, Node* &begin)
{
	Node* NewNode = new Node;		//Insert new node at right after begin
	NewNode->m_key = key;
	NewNode->m_value = value;
	NewNode->m_next = begin;
	begin = NewNode;
}

template <typename KeyType, typename ValueType>
void HashTable<KeyType, ValueType>::insertInFront1(const KeyType& key, const ValueType& value, Node1* &begin)
{
	Node1* NewNode = new Node1;			//Insert new node at right after begin
	NewNode->m_key = key;
	NewNode->m_value = value;
	NewNode->m_next = begin;
	if (begin == nullptr)
	{
		m_listTail = NewNode;
	}
	else
		begin->m_prev = NewNode;
	NewNode->m_prev = nullptr;
	begin = NewNode;
}


template <typename KeyType, typename ValueType>
void HashTable<KeyType, ValueType>::udpdateAssociation(const KeyType& key, const ValueType& value)
{
	Node1* p = m_listHead;
	while (p != nullptr && p->m_key != key)			//Check if the key already exists 
	{
		p = p->m_next;
	}

	if (p == nullptr)								//If not present, Insert in front
	{
		insertInFront1(key, value, m_listHead);
	}
	else											//Otherwise update value and touch
	{
		if (p->m_value != value)
			p->m_value = value;
		touch(key);
	}
}

template <typename KeyType, typename ValueType>
bool HashTable<KeyType, ValueType>::discard(KeyType& key, ValueType& value)
{
	if (m_listHead == nullptr && m_listTail == nullptr)  //If list empty
	{
		return false;
	}
	else
	{
		Node1* p = m_listTail;
		Node1* q = p->m_prev;
		Node* r = isPresent(p->m_key);
		unsigned int bucket = getBucketVal(p->m_key);
		if (r->m_next == nullptr)				//Remove Item from Hash Table and adjust pointers
		{
			delete r;
		}
		else
		{
			Node* l = m_table[bucket];
			while (l->m_next != r)
				l = l->m_next;
			delete r;
			l->m_next = nullptr;
		}

		if (p != m_listHead)			//Remove Item from Association Tracking List and adjust pointers
		{
			key = p->m_key;
			value = p->m_value;
			delete p;
			q->m_next = nullptr;
			m_listTail = q;
		}
		return true;
	}
}

#endif //HASHTABLE INCLUDED
