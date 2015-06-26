//
//  newMultiset.cpp
//  1. multiset
//
//  Created by Chang Liu on 1/19/15.
//  Copyright (c) 2015 Chang Liu. All rights reserved.
//

#include "newMultiset.h"

Multiset::Multiset() {
    m_array = new Item[DEFAULT_MAX_ITEMS];
    m_maxSize = DEFAULT_MAX_ITEMS;
    m_uniqueSize = 0;
}

Multiset::Multiset(int size) {
    m_array = new Item[size];
    m_maxSize = size;
    m_uniqueSize = 0;
}

Multiset::Multiset(const Multiset& copy) {
    *this = copy;
}

Multiset& Multiset::operator=(const Multiset& copy) {
    delete [] this->m_array;
    Multiset(copy.m_maxSize);
    for (int i = 0; i < copy.uniqueSize(); i++)
    {
        this->m_array[i] = copy.m_array[i];
        this->m_uniqueSize++;
    }
    
    return *this;
}

Multiset::~Multiset() {
    delete[] m_array;
}

bool Multiset::empty() {
    if (this->m_uniqueSize == 0)
        return true;
    else
        return false;
}

int Multiset::size() const {
    int size = 0;
    for (int i = 0; i < this->uniqueSize(); i++)
    {
        size += this->m_array[i].m_count;
    }
    
    return size;
}

int Multiset::uniqueSize() const {
    
    return m_uniqueSize;
}

bool Multiset::insert(const ItemType& value) {
    int index = this->find(value);
    if (index != -1)
    {
        this->m_array[index].m_count++;
        return true;
    }
    
    if (this->uniqueSize() >= m_maxSize)
    {
        return false;
    }
    
    this->m_array[m_uniqueSize].m_word = value;
    this->m_array[m_uniqueSize].m_count = 1;
    this->m_uniqueSize++;
    
    return true;
}

int Multiset::erase(const ItemType& value) {
    int index = this->find(value);
    if (index != -1)
    {
        this->m_array[index].m_count--;
        if (this->m_array[index].m_count == 0)
        {
            this->remove(index);
            return 1;
        }
        return 1;
    }
    return 0;
}

int Multiset::eraseAll(const ItemType& value) {
    int index = this->find(value);
    int count;
    if (index != -1)
    {
        count = this->m_array[index].m_count;
        this->remove(index);
        return count;
    }
    
    return 0;
}

bool Multiset::contains(const ItemType& value) const {
    if (this->find(value) != -1)
        return true;
    
    return false;
}

int Multiset::count(const ItemType& value) const {
    for (int i = 0; i < this->m_uniqueSize; i++)
    {
        if (m_array[i].m_word == value) {
            return m_array[i].m_count;
        }
    }
    return 0;
}

int Multiset::get(int i, ItemType& value) const {
    if (i >= 0 && i < this->uniqueSize())
    {
        value = this->m_array[i].m_word;
        return this->m_array[i].m_count;
        
    }
    
    return 0;
}

void Multiset::swap(Multiset& other) {
    Item* tmpArr = m_array;
    int tmpSize = this->uniqueSize();
    int tmpMax = this->m_maxSize;
    
    this->m_array = other.m_array;
    this->m_uniqueSize = other.m_uniqueSize;
    this->m_maxSize = other.m_maxSize;
    
    other.m_array = tmpArr;
    other.m_uniqueSize = tmpSize;
    other.m_maxSize = tmpMax;
}

bool Multiset::remove(int index) {
    if (index >= 0 && index < this->uniqueSize())
    {
        for (int i = index; i < this->uniqueSize(); i++)
        {
            this->m_array[i] = this->m_array[i+1];
        }
        this->m_uniqueSize--;
        return true;
    }
    return false;
}

int Multiset::find(ItemType value) const {
    for (int i = 0; i < this->uniqueSize(); i++)
    {
        if (this->m_array[i].m_word == value)
        {
            return i;
        }
    }
    return -1;
}