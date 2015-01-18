//
//  main.cpp
//  multiset
//
//  Created by Chang Liu on 1/14/15.
//  Copyright (c) 2015 Chang Liu. All rights reserved.
//

#include <iostream>
#include <string>
#include "Multiset.h"
#include <cassert>


using namespace std;

int main() {
    
    Multiset ms1;
    ms1.insert("cumin");
    ms1.insert("cumin");
    ms1.insert("cumin");
    ms1.insert("turmeric");
    Multiset ms2;
    ms2.insert("coriander");
    ms2.insert("cumin");
    ms2.insert("cardamom");
    ms1.swap(ms2);  // exchange contents of ms1 and ms2
    assert(ms1.size() == 3  &&  ms1.count("coriander") == 1  &&
           ms1.count("cumin") == 1  &&  ms1.count("cardamom") == 1);
    assert(ms2.size() == 4  &&  ms2.count("cumin") == 3  &&
           ms2.count("turmeric") == 1);
}
