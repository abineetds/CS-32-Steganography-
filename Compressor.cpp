#include "provided.h"
#include "HashTable.h"
#include <string>
#include <vector>

using namespace std;

void Compressor::compress(const string& s, vector<unsigned short>& numbers)
{
	unsigned short Capacity = (s.size() / 2) + 512;		// Assign Capacity for Hash Table
	if (Capacity > 16384)
		Capacity = 16384;
	
	HashTable<string, unsigned short> H(2 * Capacity, Capacity);	//Create Hash Table
	for (int j = 0; j < 256; j++)				//Add first characters corresponding to first 256 ASCII codes
	{
		string str(1, static_cast<char>(j));		
		H.set(str, j, true);
	}

	int nextFreeID = 256;
	string runSoFar = "";
	vector<unsigned short> V;

	for (int i = 0; i < s.size(); i++)		//Add Similar lengths of code with common value in order to compress repitition
	{
		unsigned short x;
		string expandedRun = runSoFar + s[i];
		if (H.get(expandedRun, x))
		{
			runSoFar = expandedRun;
		}
		else
		{
			H.get(runSoFar, x);
			V.push_back(x);
			H.touch(runSoFar);
			runSoFar = "";
			unsigned short cv;
			string s1(1, '\0');
			s1[0] = s[i];
			H.get(s1, cv);
			V.push_back(cv);
			if (H.set(expandedRun, nextFreeID))
			{
				nextFreeID++;
			}
			else
			{
				string key;
				unsigned short discarded;
				H.discard(key, discarded);
				H.set(expandedRun, discarded);
			}
		}
	}
	if (runSoFar != "")			//Add Last element
	{
		unsigned short x;
		H.get(runSoFar, x);
		V.push_back(x);
	}
	V.push_back(Capacity);		//Add Capacity as last element
	numbers = V;
}

bool Compressor::decompress(const vector<unsigned short>& numbers, string& s)
{
	if (numbers.size() == 1)			// Assign Capacity for Hash Table
		return false;

	unsigned short Capacity = numbers[numbers.size() - 1];
	HashTable<unsigned short, string> H(Capacity, 2 * Capacity);		//Create Hash Table
	for (unsigned short j = 0; j < 256; j++)		//Add first characters corresponding to first 256 ASCII codes
	{
		string str(1, static_cast<char>(j));
		H.set(j, str, true);
	}

	int nextFreeID = 256;
	string runSoFar = "";
	string output = "";

	for (int i = 0; i < numbers.size() - 1; i++)   //Add Similar lengths of code with common value in order to compress repitition
	{
		int us = numbers[i];

		if (us <= 255)
		{
			string val;
			H.get(us, val);
			output += val;

			if (runSoFar == "")
			{
				runSoFar += val;
				continue;
			}
			else
			{
				string expandedRun = runSoFar + val;
				if (!H.isFull())
				{
					H.set(nextFreeID, expandedRun);
					nextFreeID++;
				}
				else
				{
					unsigned short x;
					string discardVal;
					H.discard(x, discardVal);
					H.set(x, expandedRun);
				}
				runSoFar = "";
			}
		}
		else
		{
			string val;
			if (!H.get(us, val))		//append value at end of output
			{
				return false;
			}
			else
			{
				H.touch(us);
				output += val;
				runSoFar = val;
			}
		}
	}
	s = output;
	return true;
}
