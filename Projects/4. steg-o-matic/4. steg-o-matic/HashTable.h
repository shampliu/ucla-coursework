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
    HashTable(unsigned int numBuckets, unsigned int capacity);
    ~HashTable();
    
    bool isFull() const;
    bool set(const KeyType&	key, const ValueType& value, bool permanent = false);
    bool get(const KeyType& key, ValueType& value)	const;
    bool touch(const KeyType& key);
    bool discard(KeyType& key,	ValueType& value);
private:

    struct Bucket {
        KeyType     m_key;
        ValueType   m_value;
        Bucket*     m_next;
        bool        m_permanent;
        
        Bucket() : m_next(nullptr), m_permanent(false) { };
        Bucket(KeyType key, ValueType val, bool permanent) : m_key(key), m_value(val), m_next(nullptr), m_permanent(permanent) { };
    };
    
    Bucket*         m_array;
    
    struct Recent {
        
        Recent(unsigned int capacity) : m_capacity(capacity), m_top(-1) {
            m_stack = new Bucket[capacity];
        };
        
        bool push(Bucket* b) {
            if (m_top == m_capacity) {
                return false;
            }
            
            ++m_top;
            m_stack[m_top] = b;
            return true;
        }
        
        bool toTop(Bucket* b) {
            // if top value is most recent, it will stay at the top
            for (int i = 0; i < m_top; i++) {
                if (m_stack[i] == b) {
                    for (int j = i; j < m_top; j++) {
                        m_stack[j] = m_stack[j+1];
                    }
                    m_stack[m_top] = b;
                    return true;
                }
            }
            return true;
        }
        
        bool pop() {
            if (m_stack[m_top] != nullptr) {
                m_stack[m_top] = nullptr;
                m_top--;
                return true;
            }
            return false;
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
unsigned int computeHash(int key)
{
    return 0;
}

/* HashTable
 ------------------------------ */
template<typename KeyType, typename ValueType>
inline
HashTable<KeyType, ValueType>::HashTable(unsigned int numBuckets, unsigned int capacity) : m_capacity(capacity), m_pairs(0) {
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
    
    Bucket* b = m_array[index];
    
    while (b != nullptr) {
        if (b->m_key == key) {
            value = b->m_value;
            return true;
        }
        else {
            b = b->m_next;
        }
    }
    
    return false;
}

template<typename KeyType, typename ValueType>
inline
bool HashTable<KeyType, ValueType>::set(const KeyType&	key, const ValueType& value, bool permanent) {
    
    unsigned int computeHash(KeyType); // prototype
    unsigned int index = computeHash(key);
    
    Bucket* b = m_array[index];
    
    for (;;) {
        if (b == nullptr) {
            // can't add anymore
            if (isFull()) {
                return false;
            }
            // add new item
            else {
                Bucket* b = new Bucket(key, value, permanent);
                m_pairs++;
                m_array[index] = b;
                if (! permanent) {
                    m_recent->push(b);
                }
                return true;
            }
        }
        // update the value, 3rd parameter doesn't matter
        else if(b->m_key == key) {
            b->m_value = value;
            if (! b->m_permanent) {
                m_recent->toTop(b);
            }
            return true;
        }
        else {
            b = b->next;
        }
    }
    
}

template<typename KeyType, typename ValueType>
inline
bool HashTable<KeyType, ValueType>::touch(const KeyType& key) {
    
    unsigned int computeHash(KeyType); // prototype
    unsigned int index = computeHash(key);
    
    Bucket* b = m_array[index];
    
    for (;;) {
        if (b == nullptr) {
            return false;
        }
        // move to top
        else if(b->m_key == key && ! b->m_permanent) {
            m_recent->toTop(b);
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
bool HashTable<KeyType, ValueType>::discard(KeyType& key, ValueType& value) {
    
    Bucket* cur = m_recent->m_stack[m_recent->m_top];
    
    // return if no non-permanent items that were recently changed
    if (cur == nullptr) {
        return false;
    }
    
    unsigned int computeHash(KeyType); // prototype
    
    key = cur->m_key;
    value = cur->m_value;
    unsigned int index = computeHash(key);
    
    Bucket* b = m_array[index];
    
    for (;;) {
        if (b == nullptr) {
            return false;
        }
        
        else if(b->m_key == key && ! b->m_permanent) {
            m_recent->pop();
            m_array[index] = b->next;
            delete b;
            return true;
        }
        else {
            b = b->next;
        }
    }
    
    return false;
    
}


#endif /* defined(_____steg_o_matic__HashTable__) */
