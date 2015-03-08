//
//  Multiset.h
//  4. fourthHomework
//
//  Created by Chang Liu on 2/13/15.
//  Copyright (c) 2015 Chang Liu. All rights reserved.
//

// Multiset.h

#ifndef MULTISET_INCLUDED
#define MULTISET_INCLUDED

#include <string>

template <typename T>
class Multiset
{
public:
    Multiset();          // Create an empty multiset.
    bool empty() const;  // Return true if the multiset is empty, otherwise false.
    
    int size() const;
    // Return the number of items in the multiset.  For example, the size
    // of a multiset containing "cumin", "cumin", "cumin", "turmeric" is 4.
    
    int uniqueSize() const;
    // Return the number of distinct items in the multiset.  For example,
    // the uniqueSize of a multiset containing "cumin", "cumin", "cumin",
    // "turmeric" is 2.
    
    bool insert(const T& value);
    // Insert value into the multiset.  Return true if the value was
    // actually inserted.  Return false if the value was not inserted
    // (perhaps because the multiset has a fixed capacity and is full).
    
    int erase(const T& value);
    // Remove one instance of value from the multiset if present.
    // Return the number of instances removed, which will be 1 or 0.
    
    int eraseAll(const T& value);
    // Remove all instances of value from the multiset if present.
    // Return the number of instances removed.
    
    bool contains(const T& value) const;
    // Return true if the value is in the multiset, otherwise false.
    
    int count(const T& value) const;
    // Return the number of instances of value in the multiset.
    
    int get(int i, T& value) const;
    // If 0 <= i < uniqueSize(), copy into value an item in the multiset
    // and return the number of instances of that item in the multiset.
    // Otherwise, leave value unchanged and return 0.
    
    void swap(Multiset& other);
    // Exchange the contents of this multiset with the other one.
    
    // Housekeeping functions
    ~Multiset();
    Multiset(const Multiset& other);
    Multiset& operator=(const Multiset& rhs);
    
private:
    // Representation:
    //   a circular doubly-linked list with a dummy node.
    //   m_head points to the dummy node.
    //   m_head->m_prev->m_next == m_head and m_head->m_next->m_prev == m_head
    //   m_uniqueSize == 0 and m_size == 0  if and only if
    //                           m_head->m_next == m_head->m_prev == m_head
    //   In addition to the dummy node, the list has m_uniqueSize nodes.
    //   Nodes are in no particular order.
    
    struct Node
    {
        T m_value;
        int      m_count;
        Node*    m_next;
        Node*    m_prev;
    };
    
    Node* m_head;
    int   m_uniqueSize;
    int   m_size;
    
    Node* find(const T& value) const;
    // Return pointer to Node whose m_value == value if there is one,
    // else m_head
    
    int doErase(const T& value, bool all);
    // Remove one or all instances of value from the multiset if present,
    // depending on the second parameter.  Return the number of instances
    // removed.
};

// Declarations of non-member functions
inline
void exchange(int& a, int& b)
{
    int t = a;
    a = b;
    b = t;
}

template <typename T>
Multiset<T>::Multiset()
: m_uniqueSize(0), m_size(0)
{
    // create dummy node
    m_head = new Node;
    m_head->m_next = m_head;
    m_head->m_prev = m_head;
}

template <typename T>
Multiset<T>::~Multiset()
{
    // Delete the m_uniqueSize non-dummy nodes plus the dummy node
    
    for (Node* p = m_head->m_prev ; m_uniqueSize >= 0; m_uniqueSize--)
    {
        Node* toBeDeleted = p;
        p = p->m_prev;
        delete toBeDeleted;
    }
}

template <typename T>
Multiset<T>::Multiset(const Multiset<T>& other)
: m_uniqueSize(other.m_uniqueSize), m_size(other.m_size)
{
    // Create dummy node; don't initialize its m_next
    
    m_head = new Node;
    m_head->m_prev = m_head;
    
    // Copy each node from the other list; each iteration will set the
    // m_next of the previous node copied
    
    for (Node* p = other.m_head->m_next ; p != other.m_head; p = p->m_next)
    {
        // Create a copy of the node p points to
        Node* pnew = new Node;
        pnew->m_value = p->m_value;
        pnew->m_count = p->m_count;
        
        // Connect the m_prev pointers
        pnew->m_prev = m_head->m_prev;
        m_head->m_prev = pnew;
        
        // Connect the previous Node's m_next
        pnew->m_prev->m_next = pnew;
    }
    
    // Connect the last Node's m_next
    m_head->m_prev->m_next = m_head;
}

template <typename T>
Multiset<T>& Multiset<T>::operator=(const Multiset<T>& rhs)
{
    if (this != &rhs)
    {
        Multiset temp(rhs);
        swap(temp);
    }
    return *this;
}

template <typename T>
bool Multiset<T>::insert(const T& value)
{
    Node* p = find(value);
    
    if (p != m_head)  // found
        p->m_count++;
    else
    {
        // Create a new node
        p = new Node;
        p->m_value = value;
        p->m_count = 1;
        
        // Insert new item at tail of list (arbitrary choice of position)
        //     Connect it to tail
        p->m_prev = m_head->m_prev;
        p->m_prev->m_next = p;
        
        //     Connect it to dummy node
        p->m_next = m_head;
        m_head->m_prev = p;
        
        m_uniqueSize++;
    }
    
    m_size++;
    return true;
}

template <typename T>
int Multiset<T>::count(const T& value) const
{
    Node* p = find(value);
    return p == m_head ? 0 : p->m_count;
}

template <typename T>
int Multiset<T>::get(int i, T& value) const
{
    if (i < 0  ||  i >= m_uniqueSize)
        return false;
    
    // Get the value at position i.  This is one way of ensuring the required
    // behavior of get:  If the Multiset doesn't change in the interim,
    // * calling get with each i in 0 <= i < size() gets each of the
    //   Multiset elements, and
    // * calling get with the same value of i each time gets the same element.
    
    // If i is closer to the head of the list, go forward to reach that
    // position; otherwise, start from tail and go backward.
    
    Node* p;
    if (i < m_uniqueSize / 2)  // closer to head
    {
        p = m_head->m_next;
        for (int k = 0; k != i; k++)
            p = p->m_next;
    }
    else  // closer to tail
    {
        p = m_head->m_prev;
        for (int k = m_uniqueSize-1; k != i; k--)
            p = p->m_prev;
    }
    
    value = p->m_value;
    return p->m_count;
}

template <typename T>
void Multiset<T>::swap(Multiset<T>& other)
{
    // swap head pointers
    Node* temp = m_head;
    m_head = other.m_head;
    other.m_head = temp;
    
    // swap uniqueSize and size
    exchange(m_uniqueSize, other.m_uniqueSize);
    exchange(m_size, other.m_size);
}

template <typename T>
typename Multiset<T>::Node* Multiset<T>::find(const T& value) const
{
    // Do a linear search through the list
    
    Node* p;
    for (p = m_head->m_next; p != m_head && p->m_value != value; p = p->m_next)
        ;
    return p;
}

template <typename T>
int Multiset<T>::doErase(const T& value, bool all)
{
    Node* p = find(value);
    
    if (p == m_head)  // not found
        return 0;
    
    int nErased = (all ? p->m_count : 1);  // number to erase
    
    // If erasing one, and there are more than one, just decrement;
    // otherwise, we're erasing all, or erasing one whose count is 1,
    // so unlink the Node from the list and destroy it
    
    if (!all  &&  p->m_count > 1)
        p->m_count--;
    else
    {
        p->m_prev->m_next = p->m_next;
        p->m_next->m_prev = p->m_prev;
        delete p;
        
        m_uniqueSize--;
    }
    
    m_size -= nErased;
    return nErased;
}



template <typename T>
void combine(const Multiset<T>& ms1, const Multiset<T>& ms2, Multiset<T>& result)
{
    // If a value occurs n1 times in ms1 and n2 times in ms2, then
    // it will occur n1+n2 times in result upon return from this function.
    
    // Guard against the case that result is an alias for ms1 or ms2
    // (i.e., that result is a reference to the same multiset that ms1 or ms2
    // refers to) by building the answer in a local variable res.  When
    // done, swap res with result; the old value of result (now in res) will
    // be destroyed when res is destroyed.
    
    Multiset<T> res(ms1);
    for (int k = 0; k < ms2.uniqueSize(); k++)
    {
        T v;
        for (int n = ms2.get(k, v); n > 0; n--)
            res.insert(v);
    }
    result.swap(res);
}

template <typename T>
void subtract(const Multiset<T>& ms1, const Multiset<T>& ms2, Multiset<T>& result)
{
    // Guard against the case that result is an alias for ms1 or ms2
    // by building the answer in a local variable res.  When done, swap res
    // with result; the old value of result (now in res) will be destroyed
    // when res is destroyed.
    
    Multiset<T> res;
    for (int k = 0; k != ms1.uniqueSize(); k++)
    {
        T v;
        int n = ms1.get(k, v);
        for (n -= ms2.count(v); n > 0; n--)
            res.insert(v);
    }
    result.swap(res);
}
// If a value occurs n1 times in ms1 and n2 times in ms2, then
// it will occur n1-n2 times in result upon return from this function
// if n1 >= n2.  If n1 <= n2, it will not occur in result.



// Inline implementations
template <typename T>
inline
int Multiset<T>::size() const
{
    return m_size;
}

template <typename T>
inline
int Multiset<T>::uniqueSize() const
{
    return m_uniqueSize;
}

template <typename T>
inline
bool Multiset<T>::empty() const
{
    return size() == 0;
}

template <typename T>
inline
int Multiset<T>::erase(const T& value)
{
    return doErase(value, false);
}

template <typename T>
inline
int Multiset<T>::eraseAll(const T& value)
{
    return doErase(value, true);
}

template <typename T>
inline
bool Multiset<T>::contains(const T& value) const
{
    return find(value) != m_head;
}

#endif // MULTISET_INCLUDED
