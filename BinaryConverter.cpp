#include "provided.h"
#include <string>
#include <vector>

using namespace std;

const size_t BITS_PER_UNSIGNED_SHORT = 16;

string convertNumberToBitString(unsigned short number);
bool convertBitStringToNumber(string bitString, unsigned short& number);

string BinaryConverter::encode(const vector<unsigned short>& numbers)
{
	std::string binary = "";
	for (int i = 0; i < numbers.size(); i++)		//Convert each character of the vector into binary
	{
		binary += convertNumberToBitString(numbers[i]); 
	}

	std::string bitString = "";
	for (int j = 0; j < binary.size(); j++)		//For every character in the binary string, convert to either tab or space.
	{
		switch (binary[j])
		{
		case '0':
			bitString += " ";
			break;
		case '1':
			bitString += "\t";
			break;
		}
	}
	return bitString;
}

bool BinaryConverter::decode(const string& bitString,
							 vector<unsigned short>& numbers)
{
	if (bitString.size() % 16 != 0)			//Check if length of bitstring is a multiple of 16
	{
		return false;
	}

	vector<string> binary;
	unsigned int wordCount = 0;
	while (wordCount * 16 < bitString.size())		//Create a verctor and push sets of 16 appropriate binary characters onto it
	{
		binary.push_back("");
		for (int i = 0; i < 16; i++)
		{
			switch (bitString[(wordCount * 16) + i])		
			{
			case ' ':
				binary[wordCount] += "0";
				break;
			case '\t':
				binary[wordCount] += "1";
				break;
			default:
				return false;
			}
		}
		wordCount++;
	}

	numbers.clear();
	for (int j = 0; j < binary.size(); j++)					//convert those sets to integers and push them onto the numbers vector
	{
		unsigned short value;
		if (!convertBitStringToNumber(binary[j], value))
		{
			return false;
		}
		else
		{
			numbers.push_back(value);
		}
	}
	return true;
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

unsigned int computeHash(const string key)
{
	unsigned int result = 0;
	for (int i = 0; i < key.size(); i++)
	{
		result = result * 7919 + key[i];
	}
	return result;
}

unsigned int computeHash(unsigned short key)
{
	key = key * 7919;
	return key;
}
