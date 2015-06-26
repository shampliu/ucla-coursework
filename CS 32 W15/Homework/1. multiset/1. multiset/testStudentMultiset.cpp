//
//  testStudentMultiset.cpp
//  1. multiset
//
//  Created by Chang Liu on 1/19/15.
//  Copyright (c) 2015 Chang Liu. All rights reserved.
//

#include "StudentMultiset.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
    StudentMultiset ms1;
    assert(ms1.add(700));
    assert(ms1.add(800));
    ms1.add(700);
    assert(ms1.size() == 3);
    ms1.print();
    
    cout << "Passed all tests";
}
