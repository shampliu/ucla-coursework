////
////  linear.cpp
////  3. thirdHomework
////
////  Created by Chang Liu on 2/8/15.
////  Copyright (c) 2015 Chang Liu. All rights reserved.
////
//
//#include <iostream>
//using namespace std;
//
//bool somePredicate(double x) {
//    
//    return x < 0;
//}
//
//// Return true if the somePredicate function returns true for at
//// least one of the array elements, false otherwise.
//bool anyTrue(const double a[], int n)
//{
//    if (n <= 0) {
//        return false;
//    }
//    if (somePredicate(a[0])) {
//        return true;
//    }
//    a++;
//    return anyTrue(a, n-1);
//}
//
//// Return the number of elements in the array for which the
//// somePredicate function returns true.
//int countTrue(const double a[], int n)
//{
//    if (n <= 0) {
//        return 0;
//    }
//    
//    if (somePredicate(a[0]) ) {
//        return 1 + countTrue(++a, --n);
//    }
//    else {
//        return countTrue(++a, --n);
//    }
//}
//
//// Return the subscript of the first element in the array for which
//// the somePredicate function returns true.  If there is no such
//// element, return -1.
//int firstTrue(const double a[], int n)
//{
//    if (n <= 0) {
//        return -1;
//    }
//    
//    if (somePredicate(a[0]) ) {
//        return 0;
//    }
//    else {
//        return 1 + firstTrue(++a, --n);
//    }
//}
//
//// Return the subscript of the smallest element in the array (i.e.,
//// the one whose value is <= the value of all elements).  If more
//// than one element has the same smallest value, return the smallest
//// subscript of such an element.  If the array has no elements to
//// examine, return -1.
//int indexOfMin(const double a[], int n)
//{
//    if (n <= 0) {
//        return -1;
//    }
//    
//    if (n == 1) {
//        return 0;
//    }
//    
//    int least = indexOfMin(a, --n);
//    if (a[least] <= a[--n]) {
//        return least;
//    }
//    else { return n-1; }
//    
//    return least;
//}
//
//// If all n2 elements of a2 appear in the n1 element array a1, in
//// the same order (though not necessarily consecutively), then
//// return true; otherwise (i.e., if the array a1 does not include
//// a2 as a not-necessarily-contiguous subsequence), return false.
//// (Of course, if a2 is empty (i.e., n2 is 0), return true.)
//// For example, if a1 is the 7 element array
////    10 50 40 20 50 40 30
//// then the function should return true if a2 is
////    50 20 30
//// or
////    50 40 40
//// and it should return false if a2 is
////    50 30 20
//// or
////    10 20 20
//bool includes(const double a1[], int n1, const double a2[], int n2)
//{
//    if (n2 <= 0 || n1 <= 0) {
//        return false;
//    }
//    if (a1[0] == a2[0]) {
//        if (n2 == 1) {
//            return true;
//        }
//        return includes(++a1, --n1, ++a2, --n2);
//    }
//    else {
//        return includes(++a1, --n1, a2, n2);
//    }
//    
//    return false;
//}
//
//int main()
//{
//    double a[6] = { 5, 4, -1, 3, 2, -3 };
//    cout << anyTrue(a, 6) << endl;
//    cout << countTrue(a, 6) << endl;
//    cout << firstTrue(a, 6) << endl;
//    cout << indexOfMin(a, 6) << endl;
//    
//    double b[7] = { 10, 50, 40, 20, 50, 40, 30 };
//    double c[3] = { 50, 20, 30 };
//    double d[3] = { 50, 40, 40 };
//    cout << includes(b, 7, c, 3) << endl;
//    cout << includes(b, 7, d, 3) << endl;
//    
//    double e[3] = { 50, 30, 20 };
//    cout << includes (b, 7, e, 3) << endl;
//    
//}
