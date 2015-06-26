//
//  sorts.cpp
//  4. fourthHomework
//
//  Created by Chang Liu on 3/3/15.
//  Copyright (c) 2015 Chang Liu. All rights reserved.
//

#include <iostream>
#include <algorithm>
#include <numeric>  // for std::accumulate
#include <vector>
#include <string>
#include <cstdlib>  // for std::rand
#include <cassert>

using namespace std;

//========================================================================
//  Set this to false to skip the insertion sort tests; you'd do this if
//  you're sorting so many items that insertion_sort would take more time
//  than you're willing to wait.

const bool TEST_INSERTION_SORT = true;

//========================================================================

//========================================================================
// Timer t;                 // create a timer
// t.start();               // start the timer
// double d = t.elapsed();  // milliseconds since timer was last started
//========================================================================

#if __cplusplus >= 201103L  // C++11

#include <chrono>

class Timer
{
public:
    Timer()
    {
        start();
    }
    void start()
    {
        m_time = std::chrono::high_resolution_clock::now();
    }
    double elapsed() const
    {
        std::chrono::duration<double,std::milli> diff =
        std::chrono::high_resolution_clock::now() - m_time;
        return diff.count();
    }
private:
    std::chrono::high_resolution_clock::time_point m_time;
};

#elif defined(_MSC_VER)  // not C++11, but Windows

#include <windows.h>

class Timer
{
public:
    Timer()
    {
        QueryPerformanceFrequency(&ticksPerSecond);
        start();
    }
    void start()
    {
        QueryPerformanceCounter(&m_time);
    }
    double elapsed() const
    {
        LARGE_INTEGER now;
        QueryPerformanceCounter(&now);
        return (1000.0 * (now.QuadPart - m_time.QuadPart)) / ticksPerSecond.QuadPart;
    }
private:
    LARGE_INTEGER m_time;
    LARGE_INTEGER ticksPerSecond;
};

#else // not C++11 or Windows, so C++98

#include <ctime>

class Timer
{
public:
    Timer()
    {
        start();
    }
    void start()
    {
        m_time = std::clock();
    }
    double elapsed() const
    {
        return (1000.0 * (std::clock() - m_time)) / CLOCKS_PER_SEC;
    }
private:
    std::clock_t m_time;
};

#endif

//========================================================================

// Here's a class that is not cheap to copy -- the vector holds a pointer
// to dynamic memory, so vector's assignment operator and copy constructor
// have to allocate storage.

// We'll simplify writing our timing tests by declaring everything public
// in this class.  (We wouldn't make data public in a class intended for
// wider use.)

typedef int IdType;

struct Student
{
    IdType id;
    double gpa;
    vector<double> grades;
    Student(IdType i) : id(i)
    {
        // create ten random grades (from 0 to 4)
        for (size_t k = 0; k < 10; k++)
            grades.push_back(rand() % 5);
        // (accumulate computes 0.0 + grades[0] + grades[1] + ...)
        gpa = accumulate(grades.begin(), grades.end(), 0.0) / grades.size();
    }
};

inline
bool compareStudent(const Student& lhs, const Student& rhs)
{
    // The Student with the higher GPA should come first.  If they have
    // the same GPA, then the Student with the smaller id number should
    // come first.  Return true iff lhs should come first.  Notice that
    // this means that a false return means EITHER that rhs should come
    // first, or there's a tie, so we don't care which comes first,
    
    if (lhs.gpa > rhs.gpa)
        return true;
    if (lhs.gpa < rhs.gpa)
        return false;
    return lhs.id < rhs.id;
}

inline
bool compareStudentPtr(const Student* lhs, const Student* rhs)
{
    // TODO: You implement this.  Using the same criteria as compareStudent,
    //       compare two Students POINTED TO by lhs and rhs.  Think about
    //	     how you can do it in one line by calling compareStudent.
    
    // Just so this will compile, we'll pretend every comparison results in
    // a tie, so there's no preferred ordering.
    return compareStudent(*lhs, *rhs);
}

bool isSorted(const vector<Student>& s)
{
    // Return true iff the vector is sorted according to the ordering
    // relationship compareStudent
    
    for (size_t k = 1; k < s.size(); k++)
    {
        if (compareStudent(s[k],s[k-1]))
            return false;
    }
    return true;
}

void insertion_sort(vector<Student>& s, bool comp(const Student&, const Student&))
{
    // TODO: Using the insertion sort algorithm (pp. 311-313), sort s
    //       according to the ordering relationship passed in as the
    //       parameter comp.
    
    // Note:  The insertion sort algorithm on pp. 312-313 of the Carrano
    // book 6th edition is incorrect; someone made a change from the 5th
    // edition and messed it up.  See http://homepage.cs.uri.edu/~carrano/WMcpp6e
    // the errata page entry for page 313.
    
    // Just to show you how to use the second parameter, we'll write code
    // that sorts only a vector of 2 elements.  (This is *not* the
    // insertion sort algorithm.)
    
    // Note that if comp(x,y) is true, it means x must end up before y in the
    // final ordering.
    
    //    if (s.size() == 2  &&  comp(s[1], s[0]))
    //        swap(s[0], s[1]);
    
    for (int unsorted = 1; unsorted < s.size(); unsorted++) {
        Student next = s[unsorted];
        int loc = unsorted;
        while (loc > 0 && comp(next, s[loc-1])) {
            s[loc] = s[loc-1];
            loc--;
        }
        s[loc] = next;
    }
    
}

// Report the results of a timing test

void report(string caption, double t, const vector<Student>& s)
{
    cout << t << " milliseconds; " << caption
    << "; first few students are\n\t";
    size_t n = s.size();
    if (n > 5)
        n = 5;
    for (size_t k = 0; k < n; k++)
        cout << " (" << s[k].id << ", " << s[k].gpa << ")";
    cout << endl;
}

int main()
{
    size_t nstudents;
    cout << "Enter number of students to sort: ";
    cin >> nstudents;
    if ( !cin  ||  nstudents <= 0)
    {
        cout << "You must enter a positive number.  Goodbye." << endl;
        return 1;
    }
    
    // Create a random ordering of id numbers 0 through nstudents-1
    vector<IdType> ids;
    for (size_t j = 0; j < nstudents; j++)
        ids.push_back(IdType(j));
    random_shuffle(ids.begin(), ids.end());  // from <algorithm>
    
    // Create a bunch of Students
    vector<Student> unorderedStuds;
    for (size_t k = 0; k < ids.size(); k++)
        unorderedStuds.push_back(Student(ids[k]));
    
    // Create a timer
    
    Timer timer;
    
    // Sort the Students using the STL sort algorithm.  It uses a variant
    // of quicksort called introsort.
    
    vector<Student> studs(unorderedStuds);
    timer.start();
    sort(studs.begin(), studs.end(), compareStudent);
    report("STL sort", timer.elapsed(), studs);
    assert(isSorted(studs));
    
    // Sort the already sorted array using the STL sort.  This should be
    // fast.
    
    timer.start();
    sort(studs.begin(), studs.end(), compareStudent);
    report("STL sort if already sorted", timer.elapsed(), studs);
    assert(isSorted(studs));
    
    if (TEST_INSERTION_SORT)
    {
        // Sort the original unsorted array using insertion sort.  This
        // should be really slow.  If you have to wait more than a minute,
        // try rerunning the program with a smaller number of Students.
        
        studs = unorderedStuds;
        timer.start();
        insertion_sort(studs, compareStudent);
        report("insertion sort if not already sorted", timer.elapsed(), studs);
        assert(isSorted(studs));
        
        // Sort the already sorted array using insertion sort.  This should
        // be fast.
        
        timer.start();
        insertion_sort(studs, compareStudent);
        report("insertion sort if already sorted", timer.elapsed(), studs);
        assert(isSorted(studs));
    }
    
    // Since Students are expensive to copy, and since the STL's sort copies
    // Students O(N log N) times, let's sort POINTERS to the Students, then
    // make one final pass to rearrange the Students according to the
    // reordered pointers.  We'll write some code; you write the rest.
    
    // Set studs to the original unsorted sequence
    studs = unorderedStuds;
    
    // Start the timing
    timer.start();
    
    // Create an auxiliary copy of studs, to faciliate the later reordering.
    // We create it in a local scope, so we also account for the destruction
    // time.
    {
        vector<Student> auxStuds(studs);
        vector<Student*> studPtrs;
        for (auto& stud : auxStuds) {
            Student* s = &stud;
            studPtrs.push_back(s);
        }
        
        sort(studPtrs.begin(), studPtrs.end(), compareStudentPtr);
        
        // TODO:  Create a vector of Student pointers, and set each pointer
        //        to point to the corresponding Student in auxStuds.
        
        // TODO:  Sort the vector of pointers using the STL sort algorithm
        //        with compareStudentPtr as the ordering relationship.
        
        // TODO:  Using the now-sorted vector of pointers, replace each Student
        //	in studs with the Students from auxStuds in the correct order.
        
        int i = 0;
        for (auto& stud : studs) {
            stud = *(studPtrs[i]);
        }
        
    } // auxStuds will be destroyed here
    
    // Report the timing and verify that the sort worked
    report("STL sort of pointers", timer.elapsed(), studs);
    assert(isSorted(studs));
}
