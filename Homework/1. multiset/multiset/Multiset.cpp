//
//  Multiset.cpp
//  multiset
//
//  Created by Chang Liu on 1/14/15.
//  Copyright (c) 2015 Chang Liu. All rights reserved.
//

#include "Multiset.h"

using namespace std;

Multiset::Multiset() {
    m_uniqueSize = 0;
}

bool Multiset::empty() {
    if (this->m_uniqueSize == 0)
        return true;
    else
        return false;
}

int Multiset::size() {
    int size = 0;
    for (int i = 0; i < this->uniqueSize(); i++)
    {
        size += this->m_array[i]->m_count;
    }
    
    return size;
}

int Multiset::uniqueSize() {
    
    return m_uniqueSize;
}

bool Multiset::insert(const ItemType& value) {
    if (this->uniqueSize() >= DEFAULT_MAX_ITEMS)
    {
        return false;
    }
    
    int index = this->find(value);
    if (index != -1)
    {
        this->m_array[index]->m_count++;
        return true;
    }
    this->m_array[m_uniqueSize] = new Item(value);
    this->m_uniqueSize++;
    
    return true;
}

int Multiset::erase(const ItemType& value) {
    int index = this->find(value);
    if (index != -1)
    {
        this->m_array[index]->m_count--;
        if (this->m_array[index]->m_count == 0)
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
        count = this->m_array[index]->m_count;
        this->remove(index);
        return count;
    }
    
    return 0;
}

bool Multiset::contains(const ItemType& value) {
    if (this->find(value) != -1)
        return true;
    
    return false;
}

int Multiset::count(const ItemType& value) {
    for (int i = 0; i < this->m_uniqueSize; i++)
    {
        if (m_array[i]->m_word == value) {
            return m_array[i]->m_count;
        }
    }
    return 0;
}

int Multiset::get(int i, ItemType& value) {
    if (i >= 0 && i < this->uniqueSize())
    {
        value = this->m_array[i]->m_word;
        return this->m_array[i]->m_count;
        
    }
    
    return 0;
}

void Multiset::swap(Multiset& other) {
    int size1 = this->uniqueSize();
    Item* copy[size1];
    
    for (int i = 0; i < size1; i++)
    {
        copy[i] = this->m_array[i];
    }
    
    int size2 = other.uniqueSize();
    for (int j = 0; j < size2; j++)
    {
        this->m_array[j] = other.m_array[j];
    }

    for (int k = 0; k < size1; k++)
    {
        other.m_array[k] = copy[k];
    }
    
    if (size1 > size2)
    {
        for (int l = size2; l < size1; l++)
        {
            this->remove(l);
        }
    }
    else
        for (int m = size1; m < size2; m++)
        {
            other.remove(m);
        }
    
    other.m_uniqueSize = size1;
    this->m_uniqueSize = size2;
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

int Multiset::find(ItemType value) {
    for (int i = 0; i < this->uniqueSize(); i++)
    {
        if (this->m_array[i]->m_word == value)
        {
            return i;
        }
    }
    return -1;
}

