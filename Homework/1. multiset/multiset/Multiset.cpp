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

int Multiset::size() const {
    int size = 0;
    for (int i = 0; i < this->uniqueSize(); i++)
    {
        size += this->m_array[i]->m_count;
    }
    
    return size;
}

int Multiset::uniqueSize() const {
    
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

bool Multiset::contains(const ItemType& value) const {
    if (this->find(value) != -1)
        return true;
    
    return false;
}

int Multiset::count(const ItemType& value) const {
    for (int i = 0; i < this->m_uniqueSize; i++)
    {
        if (m_array[i]->m_word == value) {
            return m_array[i]->m_count;
        }
    }
    return 0;
}

int Multiset::get(int i, ItemType& value) const {
    if (i >= 0 && i < this->uniqueSize())
    {
        value = this->m_array[i]->m_word;
        return this->m_array[i]->m_count;
        
    }
    
    return 0;
}

void Multiset::swap(Multiset& other) {
    int size1 = this->uniqueSize();
    int size2 = other.uniqueSize();
    int smallerSize;
    
    if (size1 <= size2)
    {
        smallerSize = size1;
    }
    else if (size1 > size2)
    {
        smallerSize = size2;
    }
    
    for (int i = 0; i < smallerSize; i++)
    {
        Item* tmp = this->m_array[i];
        this->m_array[i] = other.m_array[i];
        other.m_array[i] = tmp;
    }
    if (smallerSize == size1)
    {
        for (int j = size1; j < size2; j++)
        {
            this->m_array[j] = other.m_array[j];
            this->remove(j);
        }
    }
    else
    {
        for (int k = size2; k < size1; k++)
        {
            other.m_array[k] = this->m_array[k];
            other.remove(k); 
        }
    }
    
    
    
//    for (int i = size1; i < size1+size2; i++)
//    {
//        this->m_array[i] = other.m_array[index];
//        index++;
//    }
//    
//    index = 0;
//    
//    for (int j = size2; j < size2 + size1; j++)
//    {
//        other.m_array[j] = this->m_array[index];
//        index++;
//    }
//    
//    for (int k = 0; k )
    
    
//    int size1 = this->uniqueSize();
//    Item* copy[size1];
//    
//    for (int i = 0; i < size1; i++)
//    {
//        copy[i] = this->m_array[i];
//    }
//    
//    int size2 = other.uniqueSize();
//    for (int j = 0; j < size2; j++)
//    {
//        this->m_array[j] = other.m_array[j];
//    }
//
//    for (int k = 0; k < size1; k++)
//    {
//        other.m_array[k] = copy[k];
//    }
//    
//    if (size1 > size2)
//    {
//        for (int l = size2; l < size1; l++)
//        {
//            this->remove(l);
//        }
//    }
//    else
//        for (int m = size1; m < size2; m++)
//        {
//            other.remove(m);
//        }
//    
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

int Multiset::find(ItemType value) const {
    for (int i = 0; i < this->uniqueSize(); i++)
    {
        if (this->m_array[i]->m_word == value)
        {
            return i;
        }
    }
    return -1;
}

