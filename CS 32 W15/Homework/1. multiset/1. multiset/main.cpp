//
//  main.cpp
//  1. multiset
//
//  Created by Chang Liu on 1/19/15.
//  Copyright (c) 2015 Chang Liu. All rights reserved.
//

#include "newMultiset.h"
#include <iostream>
#include <cassert>
using namespace std;

void test()
{
    Multiset ulms;
    assert(ulms.insert(20));
    assert(ulms.insert(10));
    assert(ulms.insert(20));
    assert(ulms.insert(30));
    assert(ulms.insert(20));
    assert(ulms.insert(10));
    assert(ulms.size() == 6  &&  ulms.uniqueSize() == 3);
    assert(ulms.count(10) == 2);
    assert(ulms.count(20) == 3);
    assert(ulms.count(30) == 1);
    assert(ulms.count(40) == 0);
}

int main()
{
    test();
    cout << "Passed all tests" << endl;
}