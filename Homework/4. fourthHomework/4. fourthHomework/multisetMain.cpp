////
////  main.cpp
////  4. fourthHomework
////
////  Created by Chang Liu on 2/13/15.
////  Copyright (c) 2015 Chang Liu. All rights reserved.
////
//
//#include "Multiset.h"
//#include <iostream>
//#include <string>
//#include <cassert>
//using namespace std;
//
//void test()
//{
//    Multiset<int> mi;
//    Multiset<string> ms;
//    assert(mi.empty());
//    assert(ms.size() == 0);
//    assert(mi.uniqueSize() == 0);
//    assert(mi.insert(10));
//    assert(ms.insert("hello"));
//    assert(mi.contains(10));
//    assert(ms.count("hello") == 1);
//    assert(mi.erase(10) == 1);
//    string s;
//    assert(ms.get(0, s)  &&  s == "hello");
//    Multiset<string> ms2(ms);
//    ms.swap(ms2);
//    ms = ms2;
//    combine(mi,mi,mi);
//    combine(ms,ms2,ms);
//    assert(ms.eraseAll("hello") == 2);
//    subtract(mi,mi,mi);
//    subtract(ms,ms2,ms);
//}
//
//int main()
//{
//    test();
//    cout << "Passed all tests" << endl;
//}
//
//#include "Multiset.h"  // class template from problem 1
//#include <string>
//using namespace std;
//
//class URL
//{
//public:
//    URL(string i) : m_id(i) {}
//    URL() : m_id("http://cs.ucla.edu") {}
//    string id() const { return m_id; }
//private:
//    string m_id;
//};
//
//int main()
//{
//    Multiset<int> mi;
//    mi.insert(7);  // OK
//    Multiset<string> ms;
//    ms.insert("http://www.symantec.com");  // OK
//    Multiset<URL> mu;
//    mu.insert(URL("http://www.symantec.com"));  // error!
//}