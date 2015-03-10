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
    HashTable(unsigned int numNodes, unsigned int capacity);
//    ~HashTable();
    
    bool isFull() const;
    bool set(const KeyType&	key, const ValueType& value, bool permanent = false);
    bool get(const KeyType& key, ValueType& value)	const;
    bool touch(const KeyType& key);
    bool discard(KeyType& key,ValueType& value);
    
private:

    struct Node {
        KeyType         m_key;
        ValueType       m_value;
        bool            m_permanent;
        
        Node*           next;
        Node*           prev;
        
        // pointer to the list
        Node*           before;
        Node*           after;
        
        Node() : next(nullptr), prev(nullptr), before(nullptr), after(nullptr) { };
//        ~Node();
    };
    
    Node**        m_array;
    
    struct List {
        List(unsigned int capacity) : m_capacity(capacity), m_head(nullptr), m_tail(nullptr) { };
        
        void insert(Node* b) {
            // first value
            if (m_head == nullptr && m_tail == nullptr) {
                m_head = m_tail = b;
                return;
            }
            // insert to top
            else {
                b->after = m_head;
                m_head->before = b;
                m_head = b;
            }
            
            m_capacity++;
        }
        
        // discard
        void remove() {
            Node* b = m_tail->before;
            b->after = nullptr;
        }
        
        // touch
        void move(Node* b) {
            if (b->before != nullptr) {
                b->before->after = b->after;
            }
            if (b->after != nullptr) {
                b->after->before = b->before;
            }
            b->after = m_head;
            m_head->before = b;
            m_head = b;
        }
    
        unsigned int    m_capacity;
        Node*           m_head;
        Node*           m_tail;
    };
    
    List*               m_list;
    
    unsigned int        m_capacity;
    unsigned int        m_pairs;
    
    //	We	prevent a	HashTable from	being	copied	or	assigned	by	declaring	the
    //	copy	constructor	and	assignment	operator	private	and	not	implementing	them.
    HashTable(const HashTable&);
    HashTable& operator=(const HashTable&);
};

// non-member functions
unsigned int computeHash(std::string key){
    return static_cast<unsigned int>(key.length());
}

unsigned int computeHash(unsigned short key) {
    return 100;
}

/* HashTable
 ------------------------------ */
template<typename KeyType, typename ValueType>
inline
HashTable<KeyType, ValueType>::HashTable(unsigned int numNodes, unsigned int capacity) : m_capacity(capacity), m_pairs(0) {
    
    m_list = new List(capacity);
    m_array = new Node*[numNodes];
    
    for (int i = 0; i < numNodes; i++) {
        m_array[i] = nullptr;
    }
    
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
    unsigned int bucket = computeHash(key);
    
    Node* b = m_array[bucket];
    
    while (b != nullptr) {
        if (b->m_key == key) {
            value = b->m_value;
            return true;
        }
        
        b = b->next;
    }
    
    // not found
    return false;
}

template<typename KeyType, typename ValueType>
inline
bool HashTable<KeyType, ValueType>::set(const KeyType& key, const ValueType& value, bool permanent) {
    
    unsigned int computeHash(KeyType); // prototype
    unsigned int bucket = computeHash(key);
    
    Node* cur = m_array[bucket];
    Node* prev = nullptr;
    
    // at least one node in Node
    if (cur != nullptr) {
        while(cur != nullptr) {
            // update the value
            if (cur->m_key == key) {
                cur->m_value = value;
                return true;
            }
            
            prev = cur;
            cur = cur->next;
        }
        
        // not found, add a new one to the end
        Node* b = new Node();
        b->m_key = key;
        b->m_value = value;
        b->m_permanent = permanent;
        
        prev->next = b;
        b->prev = prev;
        
        m_pairs++;
        
        if (! b->m_permanent) {
            m_list->move(b);
        }
        return true;
        
    }
    else {
        // can't add anymore
        if (isFull()) {
            return false;
        }
        
        // add new one
        Node* b = new Node;
        b->m_key = key;
        b->m_value = value;
        b->m_permanent = permanent;
        
        m_array[bucket] = b;
        m_pairs++;
        
        if (! permanent) {
            m_list->insert(b);
        }
        
//        std::cout << b->m_key << " , " << b-> m_value << std::endl;
        
        return true;
    }
    
}

template<typename KeyType, typename ValueType>
inline
bool HashTable<KeyType, ValueType>::touch(const KeyType& key) {
    
    unsigned int computeHash(KeyType); // prototype
    unsigned int bucket = computeHash(key);
    
    Node* b = m_array[bucket];
    
    while (b != nullptr) {
        if (b->m_key == key) {
            m_list->move(b);
            return true;
        }
        
        b = b->next;
    }
    // could not find
    return false;
    
}

template<typename KeyType, typename ValueType>
inline
bool HashTable<KeyType, ValueType>::discard(KeyType& key, ValueType& value) {
    
    Node* bottom = m_list->m_tail;
    
    if (bottom != nullptr) {
        key = bottom->m_key;
//        std::cout << key;
        value = bottom->m_value;
//        std::cout << value;
        
        m_list->remove();
    }
    // return if no non-permanent items that were recently changed
    else {
        return false;
    }
    
    unsigned int computeHash(KeyType); // prototype
    unsigned int bucket = computeHash(key);
    
    Node* b = m_array[bucket];
    while (b != nullptr) {
        if (b->m_key == key) {
            if (b->prev != nullptr) {
                b->prev->next = b->next;
            }
            if (b->next != nullptr) {
                b->next->prev = b->prev;
            }
            delete bottom;
            m_pairs--;
            return true;
        }
        
        b = b->next;
    }
    
    return false;
}


#endif /* defined(_____steg_o_matic__HashTable__) */
