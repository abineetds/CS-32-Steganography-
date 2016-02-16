#include "provided.h"
#include<vector>
#include <string>
//#include <iostream>
using namespace std;

bool Steg::hide(const string& hostIn, const string& msg, string& hostOut) 
{
	if (hostIn.size() == 0)			//If empty host, return false.
	{
		return false;
	}

	vector<string> V;
	string line = "";
	int lineCount = 0;
	for (int i = 0; i < hostIn.size(); i++)		//Extract each line out of the host and store it in a vector.
	{
		if (hostIn[i] == '\n')
		{
			lineCount++;
			V.push_back(line);
			line = "";
		}
		else
		{
			line += hostIn[i];
		}
	}
	if (line != "")							//Insert last found line as well
		V.push_back(line);

	for (int i = 0; i < V.size(); i++)			// Trace and remove trailing tabs and spaces
	{
		int end = V[i].size() - 1;
		while ((V[i][end] == '\t' || V[i][end] == ' ') && end >= 0)
		{
			V[i][end--] = '\0';
		}
	}

	vector<unsigned short> numbers;
	Compressor::compress(msg, numbers);
	string hiddenMsg = BinaryConverter::encode(numbers);
	

	int L = hiddenMsg.size();
	int N = V.size();
	int separate = (L % N);
	int j = 0;
	for (int i = 0; i < V.size() && j < hiddenMsg.size(); i++)		//Append number of characters of hidden message to the back of the stripped lines
	{
		if (i < separate)
		{
			for (int k = 0; k < ((L / N) + 1); k++)
			{
				V[i] += hiddenMsg[j++];
			}
		}
		else
		{
			for (int k = 0; k < (L / N); k++)
			{
				V[i] += hiddenMsg[j++];
			}
		}
	}

	for (int m = 0; m < V.size(); m++)		//Merge the lines in the vector
	{
		hostOut += V[m];
		hostOut += '\n';
	}
	return true;
}

bool Steg::reveal(const string& hostIn, string& msg) 
{
	if (hostIn.size() == 0)					//If HostIn is empty, return false
	{
		return false;
	}

	vector<string> V;
	string line = "";
	int lineCount = 0;
	for (int i = 0; i < hostIn.size(); i++)		//extract each line from hast
	{
		if (hostIn[i] == '\n')
		{
			lineCount++;
			V.push_back(line);
			line = "";
		}
		else
		{
			line += hostIn[i];
		}
	}
	if (line != "")
		V.push_back(line);

	string hiddenMessage;
	for (int i = 0; i < V.size(); i++)				//Extract trailing tabs and spaces from each line and save them in a string
	{
		int end = V[i].size() - 1;
		while ((V[i][end] == '\t' || V[i][end] == ' ') && end >= 0)
		{
			end--;
		}

		hiddenMessage += V[i].substr(end + 1);
	}

	vector<unsigned short> numbers;
	string message;
	if (!BinaryConverter::decode(hiddenMessage, numbers))			//Get correponding vector of unsigned shorts for this string 
	{
		//cout << "It's the Binary Converter!" << endl;
		return false;
	}
	else
	{
		if (!Compressor::decompress(numbers, msg))					// Decompress it and store it into message.
		{
			//cout << "It's the Decompressor!"<<endl;
			return false;
		}
		//cout << "secret message  " << msg << endl;
	}
	return true;
}
