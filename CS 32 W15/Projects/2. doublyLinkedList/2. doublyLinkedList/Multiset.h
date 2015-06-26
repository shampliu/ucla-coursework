//
//  Multiset.h
//  2. doublyLinkedList
//
//  Created by Chang Liu on 1/21/15.
//  Copyright (c) 2015 Chang Liu. All rights reserved.
//

#ifndef _____doublyLinkedList__Multiset__
#define _____doublyLinkedList__Multiset__

#include <iostream>
#include <string>

typedef std::string ItemType;
//typedef unsigned long ItemType;

class Multiset
{
public:
    Multiset();
    Multiset(const Multiset& src);
    Multiset& operator=(const Multiset& src);
    ~Multiset();
    
    bool empty() const;
    int size() const;
    int uniqueSize() const;
    bool insert(const ItemType& value);
    int erase(const ItemType& value);
    int eraseAll(const ItemType& value);
    bool contains(const ItemType& value) const;
    int count(const ItemType& value) const;
    int get(int i, ItemType& value) const;
    void swap(Multiset& other);
    
private:
    struct Node {
        ItemType value;
        int count;
        Node* next;
        Node* prev;
    };
    
    int m_uniqueSize;
    Node* head;
    Node* tail;
    
    Node* find(const ItemType& search);
    void remove(Node* p);
};

void combine(const Multiset& ms1, const Multiset& ms2, Multiset& result);

void subtract(const Multiset& ms1, const Multiset& ms2, Multiset& result);

#endif /* defined(_____doublyLinkedList__Multiset__) */
