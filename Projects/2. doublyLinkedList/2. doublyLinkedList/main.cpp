//
//  main.cpp
//  2. doublyLinkedList
//
//  Created by Chang Liu on 1/21/15.
//  Copyright (c) 2015 Chang Liu. All rights reserved.
//

#include "Multiset.h"
#include <iostream>
#include <cassert>
using namespace std;

void test()
{
    // default constructor
    Multiset ms1;
    
    Multiset ms5 = ms1;
    // test size
    assert(ms1.size() == 0);
    // test empty
    assert(ms1.empty());
    assert(ms1.erase("cumin") == 0);
    ms1.insert("cumin");
    ms1.insert("cumin");
    ms1.insert("Cumin");
    ms1.insert("cumiN");
    assert(ms1.size() == 4 && ms1.uniqueSize() == 3);
    ms1.insert("erasethis");
    // should decrease unique size
    ms1.erase("erasethis");
    assert(ms1.size() == 4 && ms1.uniqueSize() == 3);

    // copy constructor
    Multiset ms2(ms1);
    assert(ms2.size() == 4 && ms2.uniqueSize() == 3);
    assert(ms2.contains("Cumin"));
    ms2.insert("cumin");
    ms2.eraseAll("cumin");
    for (int i = 0; i < 3; i++)
    {
        ms2.insert("oregano");
    }
    assert(ms2.count("oregano") == 3);

    // assignment operator
    ms1 = ms2;
    assert(ms1.count("oregano") == 3);
    
    Multiset ms3;
    // swap function
    ms3.swap(ms2);
    assert(ms2.empty() && ms3.count("oregano") == 3);
    
    ms2.insert("stuff");
    ms2.insert("moreStuff");
    // combine function
    combine(ms3, ms1, ms2);
    assert(ms2.count("oregano") == 6);
    
    Multiset ms4;
    ms4.insert("oregano");
    ms4.insert("cheddar");
    ms4.insert("oregano");
    ms4.insert("Oregano");
    // subtract function
    subtract(ms2, ms4, ms3);
    assert(ms3.count("oregano") == 4); 
    
}

int main()
{
    test();
    cout << "Passed all tests" << endl;
}
