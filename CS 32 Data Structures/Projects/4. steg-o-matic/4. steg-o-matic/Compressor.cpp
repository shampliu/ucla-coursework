#include "provided.h"
#include <string>
#include <vector>
#include "HashTable.h"
#include <iostream>
#include <algorithm>

// magic constants for computeHash function
static const unsigned int InitialFNV = 2166136261U;
static const unsigned int FNVMultiple = 16777619;

using namespace std;

// non-member functions
unsigned int computeHash(std::string key){
    unsigned int hash = InitialFNV;
    for(unsigned int i = 0; i < key.length(); i++)
    {
        hash = hash ^ (key[i]);       /* xor  the low 8 bits */
        hash = hash * FNVMultiple;    /* multiply by the magic number */
    }
    return hash;
}

unsigned int computeHash(unsigned short key) {
    key = ((key >> 16) ^ key) * 0x45d9f3b;
    key = ((key >> 16) ^ key) * 0x45d9f3b;
    key = ((key >> 16) ^ key);
    return key;
}

void Compressor::compress(const string& s, vector<unsigned short>& numbers)
{
    numbers.clear();
    
    unsigned int cap = min((static_cast<int>(s.length()))/2 + 512, 16384);
    
    // max load factor is 0.5, so number of buckets should be double the cap
    HashTable<string, unsigned short> hash(cap * 2 ,cap);
    for (unsigned short i = 0; i < 256; i++) {
        string str(1, static_cast<char>(i));
        hash.set(str, i, true);
    }
    
    unsigned short nextFreeID = 256;
    string runSoFar = "";
    vector<unsigned short> V;
    
    unsigned short val;
    
    for (const char& c : s) {
        string expandedRun = runSoFar + c;

        if (hash.get(expandedRun, val)) {
            runSoFar = expandedRun;
            continue;
        }
        else {
            if (hash.get(runSoFar, val)) {
                V.push_back(val);
                hash.touch(runSoFar);
//                cout << val << " - " << runSoFar << endl;
                
            }
            else {
                cout << "Error! runSoFar not found" << endl;
            }
            runSoFar = "";
            
            if (hash.get(string(1, c), val)) {
                V.push_back(val);
//                cout << val << " - " << string(1,c) << endl;
            }
            else {
                cout << "Error! char not found" << endl;
            }
            
            if (! hash.isFull()) {
//                cout << expandedRun << " MAPPED TO " << nextFreeID << endl;
                hash.set(expandedRun, nextFreeID);
                nextFreeID++;
            }
            else {
                string id = "";
                hash.discard(id, val);
                hash.set(expandedRun, val);
            }
        }
    }
    
    if (runSoFar != "") {
        hash.get(runSoFar, val);
        V.push_back(val);
//        cout << val << " - " << runSoFar << endl;
    }
    
    // append capacity as the last number in the vector
    V.push_back(cap);
    
    numbers = V;
//    cout << "COMPRESSED NUMBER SIZE (with cap at end): " << numbers.size() << endl;
}

bool Compressor::decompress(const vector<unsigned short>& numbers, string& s)
{
    unsigned int cap = numbers[numbers.size()-1];
    HashTable<unsigned short, string> hash(cap * 2, cap);
    
//    for (auto num : numbers) {
//        cout << num << endl;
//    }
    
    for (unsigned short j = 0; j < 256; j++) {
        string str(1, static_cast<char>(j));
        hash.set(j, str, true);
    }
    
    unsigned short nextFreeID = 256;
    string runSoFar = "";
    string output = "";
    
//    cout << numbers[10] << "!!!" << endl;
    
    for (unsigned short us = 0; us < numbers.size()-1; us++) {
        // equals a 1-character string
        if (numbers[us] <= 255) {
            string val = "";
            hash.get(numbers[us], val);
            output += val;
            
            if (runSoFar == "") {
                runSoFar = val;
                continue;
            }
            else {
                string expandedRun = runSoFar + val;
                // hash isn't full
                if (!hash.isFull()) {
                    hash.set(nextFreeID, expandedRun);
                    nextFreeID++;
                }
                else {
                    unsigned short key;
                    hash.discard(key, val);
                    hash.set(key, expandedRun);
                }
                runSoFar = "";
                continue;
            }
        }
        // us represents multicharacter string
        else {
            string val = "";
            if (! hash.get(numbers[us], val)) {
                return false;
            }
            else {
                hash.touch(numbers[us]);
                output += val;
                runSoFar = val;
                continue;
            }
        }
    }
    
    s = output; 
    
    return true;
    
}
