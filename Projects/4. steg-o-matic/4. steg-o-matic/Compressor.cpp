//#include "provided.h"
//#include <string>
//#include <vector>
//#include "HashTable.h"
//
//
//using namespace std;
//
//void Compressor::compress(const string& s, vector<unsigned short>& numbers)
//{
//    unsigned int cap = min((static_cast<int>(s.length()))/2 + 512, 16384);
//    
//    // max load factor is 0.5, so number of buckets should be double the cap
//    HashTable<string, unsigned short> hash(cap * 2 ,cap);
//    for (unsigned short i = 0; i < 256; i++) {
//        string str(1, static_cast<char>(i));
//        hash.set(str, i, true);
//    }
//    
//    unsigned short nextFreeID = 256;
//    string runSoFar = "";
//    vector<unsigned short> result;
//    
//    unsigned short val;
//    
//    for(const char& c : s) {
//        string expandedRun = runSoFar + c;
//        
//        if (hash.get(s, val)) {
//            runSoFar = expandedRun;
//            continue;
//        }
//        else {
//            hash.get(runSoFar, val);
//            result.push_back(val);
//            hash.touch(runSoFar);
//            runSoFar = "";
//            
//            hash.get(static_cast<string>(&c), val);
//            result.push_back(val);
//            
//            if (! hash.isFull()) {
//                hash.set(expandedRun, nextFreeID);
//                nextFreeID++;
//            }
//            else {
//                string id = "";
//                hash.discard(id, val);
//                hash.set(expandedRun, val);
//            }
//        }
//    }
//    
//    if (runSoFar != "") {
//        hash.get(runSoFar, val);
//        result.push_back(val);
//    }
//    
//    // append capacity as the last number in the vector
//    result.push_back(cap);
//}
//
//bool Compressor::decompress(const vector<unsigned short>& numbers, string& s)
//{
//	return false;  // This compiles, but may not be correct
//}
