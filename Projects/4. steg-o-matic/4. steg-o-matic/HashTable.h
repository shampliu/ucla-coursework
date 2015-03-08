//
//  HashTable.h
//  4. steg-o-matic
//
//  Created by Chang Liu on 3/6/15.
//  Copyright (c) 2015 Chang Liu. All rights reserved.
//

#ifndef _____steg_o_matic__HashTable__
#define _____steg_o_matic__HashTable__

template <typename KeyType,	typename ValueType>
class HashTable
{
public:
    HashTable(unsigned int numBuckets, unsigned int capacity) : m_array[numBuckets], m_capacity(capacity), m_pairs(0) { };
    ~HashTable();
    
    bool isFull() const {
        if (m_capacity == m_size) {
            return true;
        }
        
        return false;
    }
    bool set(const KeyType&	key, const ValueType& value, bool permanent = false);
    bool get(const KeyType& key, ValueType& value)	const;
    bool touch(const KeyType& key);
    bool discard(KeyType& key,	ValueType& value);
private:
    //	We	prevent a	HashTable from	being	copied	or	assigned	by	declaring	the
    //	copy	constructor	and	assignment	operator	private	and	not	implementing	them.
    struct Bucket {
        KeyType     m_key;
        ValueType   m_value;
        Bucket*     m_next;
        bool        m_permanent;
        
        Bucket(KeyType key, ValueType val, bool permanent) : m_key(key), m_value(val), m_next(nullptr), m_permanent(permanent) { };
    };
    
    Bucket*         m_array[unsigned int buckets];
    
    struct Recent {
        Recent(unsigned int capacity) : m_stack[capacity], m_top(0) { };
        Bucket*     m_stack[unsigned int capacity];
        
        int         m_top;
    };
    
    Recent*         m_recent;
    
    unsigned int m_capacity;
    unsigned int m_pairs;
    
    HashTable(const HashTable&);
    HashTable& operator=(const HashTable&);
};

// non-member functions
unsigned int computeHash(int key)
{
    return 0;
}

template<typename KeyType, typename ValueType>
bool HashTable<KeyType, ValueType>::get(const Keytype& key, ValueType& value) const {
    
    unsigned int computeHash(KeyType); // prototype
    unsigned int index = computeHash(key);
    
    Bucket* b = m_array[index];
    
    while (b != nullptr) {
        if (b->m_key == key) {
            value = b->m_value;
            return true;
        }
        else {
            b = b->next;
        }
    }
    
    return false;
}

template<typename KeyType, typename ValueType>
inline
bool HashTable<KeyType, ValueType>::set(const KeyType&	key, const ValueType& value, bool permanent = false) {
    
    unsigned int computeHash(KeyType); // prototype
    unsigned int index = computeHash(key);
    
    Bucket* b = m_array[index];
    
    for (;;) {
        if (b == nullptr) {
            // can't add anymore
            if (isFull) {
                
            }
            // add new item
            else {
                Bucket* b = new Bucket(key, value, permanent)
                m_pairs++;
                
            }
            
        }
        // update the value
        else if(b->m_key == key) {
            b->m_value = value;
            return true;
        }
        else {
            b = b->next;
        }
    }
    
}



#endif /* defined(_____steg_o_matic__HashTable__) */
