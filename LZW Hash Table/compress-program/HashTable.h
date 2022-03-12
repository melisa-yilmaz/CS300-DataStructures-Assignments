//
//  hashTable.h
//  compress
//
//  Created by Melisa Yilmaz on 14.11.2021.
#ifndef HashTable_h
#define HashTable_h
#include <vector>
#include <string>
using namespace std;

struct OtherHashedObj
{
    string key; 
    int code;
    OtherHashedObj(string key= "",int code=NULL)
        :key(key), code(code) {}
    
    bool operator==(const OtherHashedObj & rhs) const
    {
        if (key != rhs.key)
        {
            return false;
        }
        return true;
    }
    bool operator!=(const OtherHashedObj & rhs) const
    {
        if (key != rhs.key)
        {
            return true;
        }
        return false;
    }
    
};


template <class HashedObj>
class HashTable
{
public:
	HashTable(const HashedObj &notFound, int size = 4099);

	HashTable(const HashTable &rhs)
		:ITEM_NOT_FOUND(rhs.ITEM_NOT_FOUND),
		hashTable(rhs.hashTable), currentSize(rhs.currentSize) {}

	const HashedObj &find(const HashedObj &x) const;

	void makeEmpty();
	void insert(const HashedObj &x);
	void remove(const HashedObj &x);
    
	enum EntryType { ACTIVE, EMPTY, DELETED };

private:
	template <class OtherHashedObj>
	struct HashEntry
	{
		OtherHashedObj element;
		EntryType info;
        HashEntry( const HashedObj & e = HashedObj( ), EntryType i = EMPTY )
                          : element(e), info(i) { }
	};

	vector<HashEntry<HashedObj>> hashTable;
	int currentSize;
	const HashedObj ITEM_NOT_FOUND;
    
    int nextPrime (int n);
    bool isPrime(int n);

	bool isActive(int currentPos) const;
	int findPos(const HashedObj &x) const;

	//void rehash();

};
int hashp(const string &x, int size);
#include "HashTable.cpp"
#endif /* HashTable_h */


