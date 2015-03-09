//
//  HashTable.h
//  4. steg-o-matic
//
//  Created by Chang Liu on 3/6/15.
//  Copyright (c) 2015 Chang Liu. All rights reserved.
//

#ifndef _____steg_o_matic__HashTable__
#define _____steg_o_matic__HashTable__

#include <string>
#include <iostream>

template <typename KeyType,	typename ValueType>
class HashTable
{
public:
    HashTable(unsigned int numBuckets, unsigned int capacity);
//    ~HashTable();
    
    bool isFull() const;
    bool set(const KeyType&	key, const ValueType& value, bool permanent = false);
    bool get(const KeyType& key, ValueType& value)	const;
    bool touch(const KeyType& key);
    bool discard(KeyType& key,ValueType& value);
    
//    const ValueType& getTop();
//    int top();
    
    
    
private:

    struct Bucket {
        KeyType     m_key;
        ValueType   m_value;
        bool        m_permanent;
        bool        m_exists;
        
        Bucket() : m_exists(false) { };
    };
    
    Bucket*         m_array;
    
    struct Recent {
        
        Recent(unsigned int capacity) : m_capacity(capacity), m_top(0) {
            m_stack = new Bucket[capacity];
        };
        
        bool push(Bucket* b) {
            if (m_top == m_capacity) {
                return false;
            }
            
            m_stack[m_top] = *b;
//            std::cout << "KEY = " << m_stack[m_top].m_key << std::endl;
            m_top++;
            return true;
        }
        
        void toTop(Bucket* b) {
            // if top value is most recent, it will stay at the top
            
            for (int i = 0; i < m_top; i++) {
                if (m_stack[i].m_key == b->m_key) {
                    
                    for (int j = i; j < m_top; j++) {
                        m_stack[j] = m_stack[j+1];
                    }
                    m_stack[m_top] = *b;
                }
            }
        }
        
        void pop() {
            m_top--;
        }
        
        Bucket*     m_stack;
        
        int         m_capacity;
        int         m_top;
    };
    
    Recent*         m_recent;
    
    unsigned int m_capacity;
    unsigned int m_pairs;
    
    //	We	prevent a	HashTable from	being	copied	or	assigned	by	declaring	the
    //	copy	constructor	and	assignment	operator	private	and	not	implementing	them.
    HashTable(const HashTable&);
    HashTable& operator=(const HashTable&);
};

// non-member functions
unsigned int computeHash(std::string key)
{
    return static_cast<unsigned int>(key.length());
}

/* HashTable
 ------------------------------ */
template<typename KeyType, typename ValueType>
inline
HashTable<KeyType, ValueType>::HashTable(unsigned int numBuckets, unsigned int capacity) : m_capacity(capacity), m_pairs(0) {
    
    m_recent = new Recent(capacity);
    m_array = new Bucket[numBuckets];
    
};

template<typename KeyType, typename ValueType>
inline
bool HashTable<KeyType, ValueType>::isFull() const {
    if (m_capacity == m_pairs) {
        return true;
    }
    
    return false;
}


template<typename KeyType, typename ValueType>
inline
bool HashTable<KeyType, ValueType>::get(const KeyType& key, ValueType& value) const {
    
    unsigned int computeHash(KeyType); // prototype
    unsigned int index = computeHash(key);
    
    Bucket* b = &m_array[index];
    
    if (b->m_exists) {
        value = b->m_value;
        return true;
    }
    else {
        return false;
    }
}

template<typename KeyType, typename ValueType>
inline
bool HashTable<KeyType, ValueType>::set(const KeyType& key, const ValueType& value, bool permanent) {
    
    unsigned int computeHash(KeyType); // prototype
    unsigned int index = computeHash(key);
    
    
    Bucket* b = &m_array[index];
//    std::cout << b->m_key << " , " << b-> m_value << std::endl;
    
    if (b->m_exists) {
        b->m_value = value;
        
        if (! b->m_permanent) {
            m_recent->toTop(b);
        }
        return true;
        
    }
    else {
        // can't add anymore
        if (isFull()) {
            return false;
        }
        
        // add new one
        Bucket* b = new Bucket;
        b->m_key = key;
        b->m_value = value;
        b->m_permanent = permanent;
        b->m_exists = true;
        
        m_pairs++;
        m_array[index] = *b;
        
        if (! permanent) {
            m_recent->push(b);
        }
        
//        std::cout << b->m_key << " , " << b-> m_value << std::endl;
        
        return true;
    }
    
}

template<typename KeyType, typename ValueType>
inline
bool HashTable<KeyType, ValueType>::touch(const KeyType& key) {
    
    unsigned int computeHash(KeyType); // prototype
    unsigned int index = computeHash(key);
    
    Bucket* b = &m_array[index];
    
    if (b->m_exists) {
        m_recent->toTop(b);
        return true;
    }
    // could not find
    else {
        return false;
    }
    
}

template<typename KeyType, typename ValueType>
inline
bool HashTable<KeyType, ValueType>::discard(KeyType& key, ValueType& value) {
    
    Bucket* top = &(m_recent->m_stack[m_recent->m_top]);
    
    unsigned int computeHash(KeyType); // prototype
    
    if (top->m_exists) {
        key = top->m_key;
        std::cout << key;
        value = top->m_value;
        std::cout << value;
        
        m_recent->pop();
        m_pairs--;
        
        delete top; 
        return true;
        
    }
    // return if no non-permanent items that were recently changed
    else {
        return false;
    }
    
}


#endif /* defined(_____steg_o_matic__HashTable__) */
