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
    
    Multiset ms;
    ms.insert("cumin");
    assert(!ms.contains(""));
    ms.insert("nutmeg");
    ms.insert("");
    ms.insert("saffron");
    assert(ms.contains(""));
    ms.erase("cumin");
    assert(ms.size() == 3  &&  ms.contains("saffron")  &&  ms.contains("nutmeg")  &&
           ms.contains(""));
}
