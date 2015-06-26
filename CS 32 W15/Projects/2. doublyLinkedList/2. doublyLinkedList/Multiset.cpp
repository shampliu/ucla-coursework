//
//  Multiset.cpp
//  2. doublyLinkedList
//
//  Created by Chang Liu on 1/21/15.
//  Copyright (c) 2015 Chang Liu. All rights reserved.
//

#include "Multiset.h"

Multiset::Multiset() {
    head = nullptr;
    tail = nullptr;
    m_uniqueSize = 0;
}

Multiset::Multiset(const Multiset& src) {
    head = nullptr;
    tail = nullptr;
    m_uniqueSize = 0;
    
    Node* p = src.head;
    while (p != nullptr)
    {
        for (int i = 0; i < p->count; i++)
        {
            this->insert(p->value);
        }
        p = p->next;
        
    }
//    *this = src;
}

Multiset& Multiset::operator=(const Multiset& src) {
    if (this == &src)
    {
        return *this;
    }
    if (head != nullptr)
    {
        Node* p = head;
        
        while (p != nullptr)
        {
            Node* n = p->next;
            this->remove(p);
            p = n;
        }
    }
    
    Node* n = src.head;
    while (n != nullptr)
    {
        for (int i = 0; i < n->count; i++)
        {
            this->insert(n->value);
//            std::cout << "inserted";
        }
        n = n->next;
    }
    
    return *this;
}

Multiset::~Multiset() {
    if (head != nullptr)
    {
        Node* temp;
        while (head != nullptr)
        {
            temp = head;
            head = head->next;
            delete temp; 
        }
    }
}

bool Multiset::empty() const {
    if (head == nullptr)
    {
        return true;
    }
    
    return false;
}

int Multiset::size() const {
    Node*p = head;
    int size = 0;
    while (p != nullptr)
    {
        size += p->count;
        p = p->next;
    }
    
    return size;
}

int Multiset::uniqueSize() const {
    return m_uniqueSize;
}

bool Multiset::insert(const ItemType& value) {
    // first value
    if (head == nullptr && tail == nullptr)
    {
        Node* p = new Node;
        p->value = value;
        p->count = 1;
        p->next = nullptr;
        p->prev = nullptr;
        head = p;
        tail = p;
        m_uniqueSize++;
        return true;
    }
    
    Node* check = this->find(value);
    
    // insert to bottom if not found
    if (check == nullptr)
    {
        Node* p = new Node;
        p->value = value;
        p->count = 1;
        tail->next = p;
        p->prev = tail;
        p->next = nullptr;
        tail = p;
        m_uniqueSize++;
    }
    // increment count if found
    else
    {
        check->count++;
    }
    
    return true;
}

int Multiset::erase(const ItemType& value) {
    Node* check = this->find(value);
    if (check != nullptr)
    {
        check->count--;
        if (check->count == 0)
        {
            this->remove(check);
        }
        
        return 1;
    }
    
    return 0;
}

int Multiset::eraseAll(const ItemType& value) {
    Node* check = this->find(value);
    if (check == nullptr)
    {
        return 0;
    }
    
    int count = check->count;
    this->remove(check);
    
    return count;
}

bool Multiset::contains(const ItemType &value) const {
    Node* p = head;
    while (p != nullptr)
    {
        if (p->value == value)
        {
            return true;
        }
        p = p->next;
    }
    
    return false;
}

int Multiset::count(const ItemType& value) const {
    Node* p = head;
    while (p != nullptr)
    {
        if (p->value == value)
        {
            return p->count;
        }
        p = p->next;
    }
    
    return 0;
}

int Multiset::get(int i, ItemType& value) const {
    // index out of range
    if (i < 0 || i >= uniqueSize())
    {
        return 0;
    }
    
    Node* p = head;
    int index = 0;
    while (index != i)
    {
        p = p->next;
        index++; 
    }
    
    value = p->value;
    
    return p->count;
}

void Multiset::swap(Multiset& other) {
    Multiset temp;
    temp = *this;
    *this = other;
    other = temp;
}

Multiset::Node* Multiset::find(const ItemType& search) {
    Node* p = head;
    while (p != nullptr)
    {
        if (p->value == search)
        {
            return p;
        }
        p = p->next;
    }
    
    return nullptr;
}

void Multiset::remove(Node* p) {
    // only value
    if (uniqueSize() == 1)
    {
        head = nullptr;
        tail = nullptr;
        delete p;
        m_uniqueSize--;
        return;
    }
    
    // head
    if (p == head)
    {
        Node* n = p->next;
        delete p;
        head = n;
        n->prev = nullptr;
        m_uniqueSize--;
        return;
    }
    
    // tail
    if (p == tail)
    {
        Node* n = p->prev;
        delete p;
        tail = n;
        n->next = nullptr;
        m_uniqueSize--;
        return;
    }
    
    // elsewhere
    Node* a = p->prev;
    Node* b = p->next;
    delete p;
    a->next = b;
    b->prev = a;
    m_uniqueSize--;
    return;
}

void combine(const Multiset& ms1, const Multiset& ms2, Multiset& result) {
    // delete result if not empty
    if (! result.empty())
    {
        Multiset emp;
        result.swap(emp);
    }
    
    for (int i = 0; i < ms1.uniqueSize(); i++)
    {
        ItemType val;
        int count = ms1.get(i, val);
        for (int j = 0; j < count; j++)
        {
            result.insert(val);
        }
    }
    
    for (int k = 0; k < ms2.uniqueSize(); k++)
    {
        ItemType val;
        int count = ms2.get(k, val);
        for (int l = 0; l < count; l++)
        {
            result.insert(val);
        }
    }
}

void subtract(const Multiset& ms1, const Multiset& ms2, Multiset& result) {
    if (! result.empty())
    {
        Multiset emp;
        result.swap(emp);
    }
    
    for (int i = 0; i < ms1.uniqueSize(); i++)
    {
        ItemType val1;
        int count1 = ms1.get(i, val1);
        
        int count2 = ms2.count(val1);
        
        if (count1 > count2)
        {
            for (int j = 0; j < count1-count2; j++)
            {
                result.insert(val1); 
            }
        }
    }
}
