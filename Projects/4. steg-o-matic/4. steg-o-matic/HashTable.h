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
        
//        Bucket() : m_permanent(false) { };
//        ~Bucket();
    };
    
    Bucket**        m_array;
    
//    struct Recent {
//        
//        Recent(unsigned int capacity) : m_capacity(capacity), m_top(0) {
//            m_list = new Bucket[capacity];
//        };
//        
//        bool push(Bucket* b) {
//            if (m_top == m_capacity) {
//                return false;
//            }
//            
//            m_list[m_top] = *b;
//
//            m_top++;
//            return true;
//        }
//        
//        void toTop(Bucket* b) {
//            // if top value is most recent, it will stay at the top
//            
//            for (int i = 0; i < m_top; i++) {
//                if (m_list[i].m_key == b->m_key) {
//                    
//                    for (int j = i; j < m_top; j++) {
//                        m_list[j] = m_list[j+1];
//                    }
//                    m_list[m_top] = *b;
//                }
//            }
//        }
//        
//        void pop() {
//            m_top--;
//        }
//        
//        Bucket*     m_list;
//        
//        int         m_capacity;
//        int         m_top;
//    };
//    
//    Recent*         m_recent;
    
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
    
    m_array = new Bucket*[numBuckets];
    
    for (int i = 0; i < numBuckets; i++) {
        m_array[i] = nullptr;
    }
//    for (int i = 0; i < numBuckets; i++) {
//        delete m_array[i];
//    }
    
//    Bucket* b = new Bucket();
//    b->m_key = "dang";
//    m_array[0] = b;
//    
//    if (m_array[0] == nullptr) {
//        std::cout << "blah";
//    }
//    else {
//        std::cout << "foo";
//    }
//    
    
    
//    m_recent = new Recent(capacity);
//    Bucket** b;
//    m_array = &b[numBuckets];
    
//    std::cout << m_array[0]->m_key;
    
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
    
    Bucket* b = m_array[index];
    
    if (b != nullptr) {
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
    
    Bucket* b = m_array[index];
    
    if (b != nullptr) {
        b->m_value = value;
//        if (! b->m_permanent) {
//            m_recent->toTop(b);
//        }
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
        
        m_pairs++;
        m_array[index] = b;
        
//        if (! permanent) {
//            m_recent->push(b);
//        }
        
//        std::cout << b->m_key << " , " << b-> m_value << std::endl;
        
        return true;
    }
    
}

template<typename KeyType, typename ValueType>
inline
bool HashTable<KeyType, ValueType>::touch(const KeyType& key) {
    
    unsigned int computeHash(KeyType); // prototype
    unsigned int index = computeHash(key);
    
    Bucket* b = m_array[index];
    
    if (b != nullptr) {
//        m_recent->toTop(b);
        return true;
    }
    // could not find
    else {
        return false;
    }
    
}

//template<typename KeyType, typename ValueType>
//inline
//bool HashTable<KeyType, ValueType>::discard(KeyType& key, ValueType& value) {
//    
//    Bucket* top = m_recent->m_stack[m_recent->m_top];
//    
//    unsigned int computeHash(KeyType); // prototype
//    
//    if (top->m_exists) {
//        key = top->m_key;
//        std::cout << key;
//        value = top->m_value;
//        std::cout << value;
//        
//        m_recent->pop();
//        m_pairs--;
//        
////        delete top;
//        top->m_exists = false;
//        return true;
//        
//    }
//    // return if no non-permanent items that were recently changed
//    else {
//        return false;
//    }
//    
//}


#endif /* defined(_____steg_o_matic__HashTable__) */
