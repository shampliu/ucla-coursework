#include "provided.h"
#include <string>
#include <vector>
#include "HashTable.h"
#include <iostream>


using namespace std;

// non-member functions
unsigned int computeHash(std::string key){
    unsigned int i, total = 0;
    
    for (i = 0; i < key.length(); i++) {
        total = total + (i+1) * key[i];
    }
    total = total & HASH_TABLE_SIZE;
    
    return total;
}

unsigned int computeHash(unsigned short key) {
    return key % HASH_TABLE_SIZE;
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
            hash.get(runSoFar, val);
            V.push_back(val);
            hash.touch(runSoFar);
            runSoFar = "";
            
            hash.get(string(1, c), val);
            V.push_back(val);
            
            if (! hash.isFull()) {
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
    }
    
    // append capacity as the last number in the vector
    V.push_back(cap);
    
    numbers = V;
    cout << "COMPRESSED NUMBER SIZE: " << numbers.size() << endl; 
}

bool Compressor::decompress(const vector<unsigned short>& numbers, string& s)
{
    unsigned int cap = numbers[numbers.size()-1];
    HashTable<unsigned short, string> hash(cap * 2, cap);
    
    for (unsigned short j = 0; j < 256; j++) {
        string str(1, static_cast<char>(j));
        hash.set(j, str, true);
    }
    
    unsigned short nextFreeID = 256;
    string runSoFar = "";
    string output = "";
    
    for (unsigned short us = 0; us < numbers.size()-1; us++) {
        if (numbers[us] <= 255) {
            string val = "";
            hash.get(numbers[us], val);
            output += val;
            
            if (runSoFar == "") {
                runSoFar = output;
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
