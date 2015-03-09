#include "provided.h"
#include <string>
#include <vector>
using namespace std;

const size_t BITS_PER_UNSIGNED_SHORT = 16;

string convertNumberToBitString(unsigned short number);
bool convertBitStringToNumber(string bitString, unsigned short& number);

string BinaryConverter::encode(const vector<unsigned short>& numbers)
{
    string result = "";
    for (auto& num : numbers) {
        result += convertNumberToBitString(num);
    }
    
    for (int i = 0; i < result.length(); i++) {
        if (i == '1') {
            result[i] = '\t';
        }
        else if (i == '0') {
            result[i] = ' ';
        }
        // shouldn't be any other character
        else {
            return "";
        }
    }
    return result;
}

bool BinaryConverter::decode(const string& bitString,
							 vector<unsigned short>& numbers)
{
    int len = static_cast<int>(bitString.length());
    if (len % 16 != 0) {
        return false;
    }
    
    // erases all elements of vector
    numbers.clear();
    
    for (int i = 0; i < len; i+=16) {
        string result = "";
        unsigned short num;
        for (int j = i; j < i+16; j++) {
            if (bitString[j] == '\t') {
                result += '1';
            }
            else if (bitString[j] == ' ') {
                result += '0';
            }
            // if not tab or space character
            else {
                return false;
            }
        }
        convertBitStringToNumber(result, num);
        numbers.push_back(num);
    }
    
	return false;  // This compiles, but may not be correct
}

string convertNumberToBitString(unsigned short number)
{
	string result(BITS_PER_UNSIGNED_SHORT, '0');
	for (size_t k = BITS_PER_UNSIGNED_SHORT; number != 0; number /= 2)
	{
		k--;
		if (number % 2 == 1)
			result[k] = '1';
	}
	return result;
}

bool convertBitStringToNumber(string bitString, unsigned short& number)
{
	if (bitString.size() != BITS_PER_UNSIGNED_SHORT)
		return false;
	number = 0;
	for (size_t k = 0; k < bitString.size(); k++)
	{
		number *= 2;
		if (bitString[k] == '1')
			number++;
		else if (bitString[k] != '0')
			return false;
	}
	return true;
}
