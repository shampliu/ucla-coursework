////
////  one.cpp
////  warmup
////
////  Created by Chang Liu on 2/17/15.
////  Copyright (c) 2015 Chang Liu. All rights reserved.
////
//
//#include <list>
//#include <vector>
//#include <algorithm>
//#include <iostream>
//#include <cassert>
//using namespace std;
//
//// Remove the odd integers from li.
//// It is acceptable if the order of the remaining even integers is not
//// the same as in the original list.
//void removeOdds(list<int>& li)
//{
//    list<int>::iterator it;
//    for (it = li.begin(); it != li.end(); it++) {
//        if (*it % 2 != 0) {
//            it= li.erase(it);
//            
//        }
//    }
//}
//
//void test()
//{
//    int a[8] = { 2, 8, 5, 6, 7, 3, 4, 1 };
//    list<int> x(a, a+8);  // construct x from the array
//    assert(x.size() == 8 && x.front() == 2 && x.back() == 1);
//    removeOdds(x);
//    assert(x.size() == 4);
//    vector<int> v(x.begin(), x.end());  // construct v from x
//    sort(v.begin(), v.end());
//    int expect[4] = { 2, 4, 6, 8 };
//    for (int k = 0; k < 4; k++)
//        assert(v[k] == expect[k]);
//    
//    
//    
//    
//}
//
//int main()
//{
//    test();
//    cout << "Passed" << endl;
//}
