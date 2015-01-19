//
//  testnewMultiset.cpp
//  1. multiset
//
//  Created by Chang Liu on 1/19/15.
//  Copyright (c) 2015 Chang Liu. All rights reserved.
//

#include "newMultiset.h"
#include <iostream>
#include <cassert>

using namespace std;

int main()
{
    Multiset a(1000);   // a can hold at most 1000 distinct items
    Multiset b(5);      // b can hold at most 5 distinct items
    Multiset c;         // c can hold at most DEFAULT_MAX_ITEMS distinct items
    ItemType v[6] = { 1, 2, 3, 4, 5, 6 };
    //    // No failures inserting 5 distinct items twice each into b
    for (int k = 0; k < 5; k++)
    {
        assert(b.insert(v[k]));
        assert(b.insert(v[k]));
    }
    assert(b.size() == 10  &&  b.uniqueSize() == 5  &&  b.count(v[0]) == 2);
    // Failure if we try to insert a sixth distinct item into b
    assert(!b.insert(v[5]));
    //
    // When two Multisets' contents are swapped, their capacities are swapped
    // as well:
    a.swap(b);
    assert(!a.insert(v[5])  &&  b.insert(v[5]));
    
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
