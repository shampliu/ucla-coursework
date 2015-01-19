//
//  testMultiset.cpp
//  1. multiset
//
//  Created by Chang Liu on 1/19/15.
//  Copyright (c) 2015 Chang Liu. All rights reserved.
//

#include "Multiset.h"
#include <cassert>
#include <iostream>

using namespace std;

int main() {
    Multiset ms1;
    assert(ms1.empty());
    unsigned long x = 999;
    assert(ms1.get(0, x) == 0  &&  x == 999);  // x unchanged by get failure
    assert( ! ms1.contains(42));
    ms1.insert(42);
    ms1.insert(42);
    assert(ms1.size() == 2  &&  ms1.uniqueSize() == 1);
    assert(ms1.get(1, x) == 0  &&  x == 999);  // x unchanged by get failure
    assert(ms1.get(0, x) == 2  &&  x == 42);
    
    Multiset ms2;
    ms2.insert(420);
    ms2.insert(420);
    ms2.insert(400);
    ms2.insert(360);
    assert(ms2.contains(360));
    assert(ms2.size() == 4 && ms2.uniqueSize() == 3);
    ms2.erase(360);
    assert(! ms2.contains(360));
    assert(ms2.size() == 3 && ms2.uniqueSize() == 2);
    ms2.eraseAll(420);
    assert(ms2.size() == 1 && ms2.uniqueSize() == 1);
    ms2.insert(500);
    ms2.insert(520);
    ms2.insert(500); // unique size should be 3, size should be 4
    
    ms2.swap(ms1);
    assert(ms1.size() == 4 && ms1.uniqueSize() == 3);
    assert(ms2.size() == 2 && ms2.uniqueSize() == 1);
    
    cout << "Passed all tests" << endl;
}
