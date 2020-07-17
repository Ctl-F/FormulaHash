#include <vector>
#include <iostream>
#include <string>
#include <cstdlib>
#include <chrono>
#include <unordered_map>
#include "FormulaHasher.h"
#include <time.h>
#include <unordered_set>

using namespace std::chrono;
inline long currentTime(){
	return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

using namespace std;
using namespace ctl_f; 
// Class: FormulaHasher
// 	string hash(string);

/**
 * Populates the given vector to max byte size of hash. Then populates it's vector's vecor to max byte size of hash.
 * Then populates it's vector and so on and so forth till the 3d vector is size maxbyte size
 *
 * input:
 * 	hashTable: A vector of vectors of vectors of strings, to be populated to max byte size
 * return: 
 * 	a populated hash tables to max size
 * */
vector<vector<vector<string>>> populateHashtable(vector<vector<vector<string>>>  &hashTable, int maxSize)
{
	for(int i= 0; i <= maxSize; i++)
	{
		vector<vector<string>> _2ndTable;
		for(int j=0; j <= maxSize; j++)
		{
			vector<string> _3rdTable;
			_2ndTable.push_back(_3rdTable);
		}
		hashTable.push_back(_2ndTable);
		
	}	
	return hashTable;
}
/**
 *
 * generates string baased on the maximum worldlength
 *
 * input:
 * 	maxWordLength: the largest possible word
 * return: 
 * 	the random string
 * */
string generateRandomStringInt(int maxWordLength)
{
	int wordLength = rand() % maxWordLength + 1;
	string randString = "";
	string alphabet =
		"0123456789"
		"QWERTYUIOPASDFGHJKLZXCVBNM"
		"qwertyuiopasdfghjklzxcvbnm,";

	for(int i = 0; i < wordLength; i++)
	{
		randString += alphabet[rand() % alphabet.length()];
	}
	return randString;
	
}
/**
 * generates a test run of the hash algorithm and reports all collisions, will tell you the total amount of collisions at the end
 *
 * input:
 * 	hashTable: a vector of vectore of vector of strings, to store the hashcodes in a table
 * 	values: the total number of values to be used during this test run
 * 	maxWordLength: maximum length of words to be used in this test run
 * 	
 * */
void  testHashtable(vector<vector<vector<string>>> &hashTable, int values, int maxWordLength, int maxSize)
{
	populateHashtable(hashTable, maxSize);
	unordered_set<string> testedInputs;
	unordered_map<string, string> hashMap;
	FormulaHasher hasher;
	int collisionCounter = 0;

	long timeStamp = currentTime();

	for(int i = 0; i < values; i++)
	{
		string word = generateRandomStringInt(maxWordLength);
		
		while(testedInputs.find(word) != testedInputs.end()){
			word = generateRandomStringInt(maxWordLength);
		}
		testedInputs.insert(word);

		string hash = hasher.hash(word);
		int indexOne = 0;
		int indexTwo = 0;
		
		if(currentTime() - timeStamp > 2500){
			cout << "Done " << i << "/" << values << "\n";
			timeStamp = currentTime();
		}

		//gets proper indexs from the hash
		indexOne += hash[0];
		indexTwo += hash[1];

		bool add = true;
		//collision check
		for(int j = 0; j < hashTable[indexOne][indexTwo].size(); j++)
		{
			if(hashTable[indexOne][indexTwo][j] == hash)
			{
				cout << "There is a collision between, " << word
					<< " and " << hashMap[hash] << endl;
				collisionCounter++;
				add = false;	
			}

		}

		if(add == true)
		{
			hashMap[hash] = word;
			hashTable[indexOne][indexTwo].push_back(hash);
		}

	}
	cout << "Testing Report: " << endl;
	cout << "Total collisions: " << collisionCounter << endl;
}

int main()
{
	srand(time(NULL));
	int values = 9999999;
	int maxWordLength = 50;
	int maxSize = 255;
	vector<vector<vector<string>>> hashTable;

	cout << "Starting Test: ";
	long startTime = currentTime();

	testHashtable(hashTable, values, maxWordLength, maxSize);
	
	long endTime = currentTime();
	cout << "Tested in " << (endTime-startTime) << "(ms)\n";

	return 0; //#
}
