//
//  StudentMultiset.h
//  1. multiset
//
//  Created by Chang Liu on 1/19/15.
//  Copyright (c) 2015 Chang Liu. All rights reserved.
//

#ifndef _____multiset__StudentMultiset__
#define _____multiset__StudentMultiset__

#include "Multiset.h"
#include <iostream>

class StudentMultiset
{
public:
    StudentMultiset();       // Create an empty student multiset.
    
    bool add(unsigned long id);
    // Add a student id to the StudentMultiset.  Return true if and only
    // if the id was actually added.
    
    int size() const;
    // Return the number of items in the StudentMultiset.  If an id was
    // added n times, it contributes n to the size.
    
    void print() const;
    // Print to cout every student id in the StudentMultiset one per line;
    // print as many lines for each id as it occurs in the StudentMultiset.
    
private:
    
    // Some of your code goes here.
    Multiset m_submissions;
    
    
};

#endif /* defined(_____multiset__StudentMultiset__) */
