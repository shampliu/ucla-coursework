//
//  Multiset.cpp
//  1. multiset
//
//  Created by Chang Liu on 1/19/15.
//  Copyright (c) 2015 Chang Liu. All rights reserved.
//

#include "Multiset.h"

using namespace std;

Multiset::Multiset() {
    m_uniqueSize = 0;
}

bool Multiset::empty() {
    if (m_uniqueSize == 0)
        return true;
    else
        return false;
}

int Multiset::size() const {
    int size = 0;
    for (int i = 0; i < uniqueSize(); i++)
    {
        size += m_array[i].m_count;
    }
    return size;
}

int Multiset::uniqueSize() const {
    return m_uniqueSize;
}

bool Multiset::insert(const ItemType& value) {
    if (uniqueSize() >= DEFAULT_MAX_ITEMS)
    {
        return false;
    }
    
    int index = find(value);
    if (index != -1)
    {
        m_array[index].m_count++;
        return true;
    }
    
    Item insert;
    insert.m_count = 1;
    insert.m_item = value;
    m_array[m_uniqueSize] = insert;
    m_uniqueSize++;
    
    return true;
}

int Multiset::erase(const ItemType& value) {
    int index = find(value);
    if (index != -1)
    {
        m_array[index].m_count--;
        if (m_array[index].m_count == 0)
        {
            remove(index);
            return 1;
        }
        return 1;
    }
    return 0;
}

int Multiset::eraseAll(const ItemType& value) {
    int index = find(value);
    int count;
    if (index != -1)
    {
        count = m_array[index].m_count;
        remove(index);
        return count;
    }
    
    return 0;
}

bool Multiset::contains(const ItemType& value) const {
    if (find(value) != -1)
        return true;
    
    return false;
}

int Multiset::count(const ItemType& value) const {
    for (int i = 0; i < m_uniqueSize; i++)
    {
        if (m_array[i].m_item == value) {
            return m_array[i].m_count;
        }
    }
    return 0;
}

int Multiset::get(int i, ItemType& value) const {
    if (i >= 0 && i < uniqueSize())
    {
        value = m_array[i].m_item;
        return m_array[i].m_count;
        
    }
    
    return 0;
}

void Multiset::swap(Multiset& other) {
    int size1 = uniqueSize();
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
        Item tmp = m_array[i];
        m_array[i] = other.m_array[i];
        other.m_array[i] = tmp;
    }
    if (smallerSize == size1)
    {
        for (int j = size1; j < size2; j++)
        {
            m_array[j] = other.m_array[j];
            remove(j);
        }
    }
    else
    {
        for (int k = size2; k < size1; k++)
        {
            other.m_array[k] = m_array[k];
            other.remove(k);
        }
    }
    
    other.m_uniqueSize = size1;
    m_uniqueSize = size2;
    
}

bool Multiset::remove(int index) {
    if (index >= 0 && index < uniqueSize())
    {
        for (int i = index; i < uniqueSize(); i++)
        {
            m_array[i] = m_array[i+1];
        }
        m_uniqueSize--;
        return true;
    }
    
    return false;
}

int Multiset::find(ItemType value) const {
    for (int i = 0; i < uniqueSize(); i++)
    {
        if (m_array[i].m_item == value)
        {
            return i;
        }
    }
    // if not found
    return -1;
}

