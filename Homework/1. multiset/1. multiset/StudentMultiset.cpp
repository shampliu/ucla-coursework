//
//  StudentMultiset.cpp
//  1. multiset
//
//  Created by Chang Liu on 1/19/15.
//  Copyright (c) 2015 Chang Liu. All rights reserved.
//

#include "StudentMultiset.h"

using namespace std;

StudentMultiset::StudentMultiset() {
    Multiset m_submissions;
}

bool StudentMultiset::add(unsigned long id) {
    if (m_submissions.insert(id))
    {
        return true;
    }
    
    return false;
}

int StudentMultiset::size() const {
    return m_submissions.size();
}

void StudentMultiset::print() const {
    for (int i = 0; i < m_submissions.uniqueSize(); i++)
    {
        ItemType item;
        int count = m_submissions.get(i, item);
        
        for (int j = 0; j < count; j++)
        {
            cout << item << endl;
        }
    }
}
