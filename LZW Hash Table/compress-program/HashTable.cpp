//
//  hashTable.cpp
//  compress
//

#include "HashTable.h"
using namespace std;

template<class HashedObj>
HashTable<HashedObj>::HashTable(const HashedObj &notFound, int size)
    :ITEM_NOT_FOUND(notFound), hashTable(nextPrime(4099))
{
    makeEmpty();
}




//Internal method to return a prime number at least as large as n.
//Assumes n > 0.
template<class HashedObj>
int HashTable<HashedObj>::nextPrime(int n)
{
    if( n % 2 == 0 )
        n++;

    for( ;!isPrime(n); n += 2 )
        ;

    return n;
 
}

//Internal method to test if a positive number is prime.
//Not an efficient algorithm.
template<class HashedObj>
bool HashTable<HashedObj>::isPrime(int n)
{
    if( n == 2 || n == 3 )
        return true;

    if( n == 1 || n % 2 == 0 )
        return false;

    for( int i = 3; i * i <= n; i += 2 )
        if( n % i == 0 )
            return false;

    return true;

}

template <class HashedObj>
void HashTable<HashedObj>::makeEmpty()
{
    for( int i = 0; i < hashTable.size(); i++ )
        hashTable[i].info = EMPTY;  // destroy the lists but not the vector
}


int hashp(const OtherHashedObj &x, unsigned int tableSize)
{
    int hashVal = 0;
    for (int i = 0; i < x.key.length(); i++)
        hashVal = 37 * hashVal + x.key[i];

    hashVal = hashVal % tableSize;

    if (hashVal < 0)
        hashVal = hashVal + tableSize;

    return(hashVal);
}


//Method that performs quadratic probing resolution.
//Return the position where the search for x terminates.
template <class HashedObj>
int HashTable<HashedObj>::findPos(const HashedObj &x) const
{
    int i = 1; //number of collusions
    int p = hashp(x, hashTable.size());

    while (hashTable[p].info != EMPTY && hashTable[p].element != x)
    {
        p += i;
        if (p >= hashTable.size())
            p -= hashTable.size();
    }
    return p;
}


//Return true if currentPos exists and is active.
template <class HashedObj>
bool HashTable<HashedObj>::isActive(int currentPos) const
{
    return hashTable[currentPos].info == ACTIVE;
}


// Remove item x from the hash table. x has to be in the table
template <class HashedObj>
void HashTable<HashedObj>::remove(const HashedObj &x)
{
    int currentPos = findPos(x);

    if (isActive(currentPos))
        hashTable[currentPos].info = DELETED;
}


//Find item x in the hash table.
//Return the matching item, or ITEM_NOT_FOUND, if not found.
template <class HashedObj>
const HashedObj &HashTable<HashedObj>::find(const HashedObj &x) const
{
    int currentPos = findPos(x);

    if (isActive(currentPos))
        return hashTable[currentPos].element;

    return  ITEM_NOT_FOUND;
}


//Insert item x into the hash table. If the item is already present, then do nothing.
template <class HashedObj>
void HashTable<HashedObj>::insert(const HashedObj &x)
{
    // Insert x as active
    int currentPos = findPos(x);

    if (isActive(currentPos))
        return;

    hashTable[currentPos].element = x;
    hashTable[currentPos].info = ACTIVE;
}



