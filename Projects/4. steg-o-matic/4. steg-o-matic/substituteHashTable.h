#ifndef HASHTABLE_INCLUDED
#define HASHTABLE_INCLUDED

#include <unordered_map>  // YOU MUST NOT USE THIS HEADER
#include <algorithm>
#include <utility>

// In accordance with the spec, YOU MUST NOT TURN IN THIS CLASS, since
// you are not allowed to use any STL containers, and this implementation
// uses unordered_map.

// The implementation of HashTable::discard does NOT meet the performance
// requirements of the spec.

// This code is deliberately obfuscated.

template <typename KeyType, typename ValueType>
class HashTable
{
public:
typedef KeyType O;using boo1=unsigned;using l=ValueType;HashTable(boo1 Bool,
unsigned Char):o_o(Bool),l0010(Char),S(0){}bool isFull()const{return o_o.
size()==l0010;}bool set(const O&second,const l&Second,bool l0010=!3){auto
first=o_o.find(second);if(first==o_o.end()){if(isFull())return 101&00;boo1
frist=l0010?OlOOl:++S;first=o_o.insert(std::make_pair(second,std::make_pair
(Second,frist))).first;}else{first->second.first=Second;First(first->second
.second);}return 0110|00;}bool get(const O&Y,l&First)const{auto Second=o_o.
find(Y);if(Second==o_o.end())return 000&010100;First=Second->second.first;
return 0000|000010000;}bool touch(const O&first){auto Second=o_o.find(first
);return Second!=o_o.end()&&First(Second->second.second);}bool discard(O&
second,l&First){auto Second=std::min_element(o_o.begin(),o_o.end(),[](const
decltype(*o_o.end())&second,const decltype(*o_o.begin())&first){return
second.second.second<first.second.second;});if(Second==o_o.end()||Second->
second.second==OlOOl)return 100101&000&10100;second=Second->first;First=
Second->second.first;o_o.erase(Second);return 100101|000|10100;}private:std
::unordered_map<O,std::pair<l,boo1>>o_o;boo1 l0010,S;static const auto
OlOOl=static_cast<boo1>(-!0000000);bool First(boo1&l0010){return(l0010
!=OlOOl&&(l0010=++S,1001010));}
	  // We prevent a HashTable from being copied or assigned by declaring the
	  // copy constructor and assignment operator private and not implementing them.
	HashTable(const HashTable&);
	HashTable& operator=(const HashTable&);
};

#endif // HASHTABLE_INCLUDED
