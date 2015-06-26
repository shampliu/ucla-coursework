//
//  main.cpp
//  3. thirdHomework
//
//  Created by Chang Liu on 2/8/15.
//  Copyright (c) 2015 Chang Liu. All rights reserved.
//

#include <iostream>

using namespace std;

// Return the number of ways that all n2 elements of a2 appear
// in the n1 element array a1 in the same order (though not
// necessarily consecutively).  The empty sequence appears in a
// sequence of length n1 in 1 way, even if n1 is 0.
// For example, if a1 is the 7 element array
//	10 50 40 20 50 40 30
// then for this value of a2     the function must return
//	10 20 40			1
//	10 40 30			2
//	20 10 40			0
//	50 40 30			3

int countIncludes(const double a1[], int n1, const double a2[], int n2)
{
    {
        if (n2 <= 0 || n1 <= 0) {
            return 0;
        }
        if (n2 >= n1) {
            return 0;
        }
        if (a1[0] == a2[0]) {
            if (n2 == 1) {
                return 1;
            }
            return countIncludes(a1+1, n1-1, a2, n2) + countIncludes(++a1, --n1, ++a2, --n2);
        }
        else {
            return countIncludes(++a1, --n1, a2, n2);
        }
        
    }
}

// Exchange two doubles
void exchange(double& x, double& y)
{
    double t = x;
    x = y;
    y = t;
}

// Rearrange the elements of the array so that all the elements
// whose value is > separator come before all the other elements,
// and all the elements whose value is < separator come after all
// the other elements.  Upon return, firstNotGreater is set to the
// index of the first element in the rearranged array that is
// <= separator, or n if there is no such element, and firstLess is
// set to the index of the first element that is < separator, or n
// if there is no such element.
// In other words, upon return from the function, the array is a
// permutation of its original value such that
//   * for 0 <= i < firstNotGreater, a[i] > separator
//   * for firstNotGreater <= i < firstLess, a[i] == separator
//   * for firstLess <= i < n, a[i] < separator
// All the elements > separator end up in no particular order.
// All the elements < separator end up in no particular order.
void separate(double a[], int n, double separator,
              int& firstNotGreater, int& firstLess)
{
    if (n < 0)
        n = 0;
    
    // It will always be the case that just before evaluating the loop
    // condition:
    //  firstNotGreater <= firstUnknown and firstUnknown <= firstLess
    //  Every element earlier than position firstNotGreater is > separator
    //  Every element from position firstNotGreater to firstUnknown-1 is
    //    == separator
    //  Every element from firstUnknown to firstLess-1 is not known yet
    //  Every element at position firstLess or later is < separator
    
    firstNotGreater = 0;
    firstLess = n;
    int firstUnknown = 0;
    while (firstUnknown < firstLess)
    {
        if (a[firstUnknown] < separator)
        {
            firstLess--;
            exchange(a[firstUnknown], a[firstLess]);
        }
        else
        {
            if (a[firstUnknown] > separator)
            {
                exchange(a[firstNotGreater], a[firstUnknown]);
                firstNotGreater++;
            }
            firstUnknown++;
        }
    }
}

// Rearrange the elements of the array so that
// a[0] >= a[1] >= a[2] >= ... >= a[n-2] >= a[n-1]
// If n <= 1, do nothing.
void order(double a[], int n)
{
    if (n <= 1) {
        return;
    }
    int f1;
    int f2;
    separate(a, n, a[0], f1, f2);
    order(a, f1);
//    order(++a, n-1);
    if(f2 < n)
    {
        order(a+f2, n-f2);     }
    
    return;
}

int main() {
    
    double a[10] = { 10, 50, 40, 20, 50, 40, 30, 90, 20, 50 };
//    double b[3] = { 10, 20, 40 };
//    double c[3] = { 10, 40, 30 };
//    double d[3] = { 20, 10, 40 };
//    double e[3] = { 50, 40, 30 };
//    cout << countIncludes(a, 7, b, 3) << endl;
//    cout << countIncludes(a, 7, c, 3) << endl;
//    cout << countIncludes(a, 7, d, 3) << endl;
//    cout << countIncludes(a, 7, e, 3) << endl;
    
    int f1;
    int f2;
    order(a, 10);
    for (int i = 0; i < 10; i++) {
        cout << a[i] << " ";
    }
    
    
}