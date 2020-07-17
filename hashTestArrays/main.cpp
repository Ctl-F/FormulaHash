#include "FormulaHasher.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

using std::cout;
using std::cin;
using std::getline;
using std::string;
using std::ifstream;
using std::vector;
using namespace std::chrono;

using ctl_f::FormulaHasher;

inline long currentTime(){
	return (long)duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

/*
	compare how many characters in the two strings are the same.
	Return as a percentage from 0-100
*/
double matchStrings(const char* first, const char* second){
	int matchedChars = 0;
	int maxIndex = FormulaHasher::HASH_SIZE;//(first.length() > second.length()) ? first.length() : second.length();
	
	if(maxIndex == 0){
		cout << "Error! Got zero!!" << "\n";
		return nan("");
	}
	
	for(int i=0; i<maxIndex; i++){
		matchedChars += first[i] == second[i];
	}
	return ((double)matchedChars / (double)maxIndex) * 100.0;
}

int addtorial(int n){
	if(n < 2){
		return n;
	}
	if(n == 2){
		return 1;
	}
	int value = 1;
	for(int i=2; i<n; i++){
		value += i;
	}
	return value;
}


// read in the file lines
vector<string> getFileLines(const string& fname) {
	ifstream f(fname);
	vector<string> lines;
	string line;

	if (f.is_open()) {
		while (f.good()) {
			getline(f, line);
			lines.push_back(line);
		}
		f.close();
	}

	return lines;
}

void generateReportFromFile(const string& fname){
	vector<string> lines = getFileLines(fname);
	cout << "Read " << lines.size() << " lines from file\n";
	
	if (lines.size() == 0) {
		return;
	}

	cout << "Hashing...\n";

	long timeStamp = currentTime();

	vector<string> hashes;
	FormulaHasher hasher;
	for (unsigned int i = 0; i < lines.size(); i++) {
		hashes.push_back(hasher.hash(lines[i]));

		if (currentTime() - timeStamp > 1000) {
			cout << i << "/" << lines.size() << "\n";
			timeStamp = currentTime();
		}
	}

	cout << "Hashing finished, beginning comparisons...\n";

	string hashA, hashB;
	double avg = 0.0, max = -100.0, min = 200.0;
	int count = 0;
	int targetCount = addtorial(hashes.size());

	long startTime = currentTime();
	for(unsigned int i=0; i<hashes.size(); i++){
		hashA = hashes[i];
		for(unsigned int j=0; j<hashes.size(); j++){
			++count;
			if(i == j) {
				continue;
			}
			hashB = hashes[j];

			double match = matchStrings(hashA.c_str(), hashB.c_str());
			if (isnan(match)) {
				cout << "Invalid Hash Alert, given\n==========\n" << hashA << "\n------------\n" << hashB << "\n==========\n";
				return;
			}

			if (match > 99.9) {
				cout << "Quase equal hash match found with " << lines[i] << " and " << lines[j] << " [" << match << "%]\n";
			}

			avg += match;
			if(match < min){
				min = match;
			}
			if(match > max){
				max = match;
			}

			if(currentTime() - timeStamp > 1000){
				cout << count << "/" << targetCount << "\n";
				timeStamp = currentTime();
			}
		}
	}
	long endTime = currentTime();
	avg /= (double)hashes.size();
	cout << "Average: " << avg << "\nMax: " << max << "\nMin: " << min << "\n";
	cout << "Time: " << (endTime - startTime) << "(ms)\n";
}

int interactiveMode();

int main() {
	cout << "Please choose a mode: ";
	string userInput;
	getline(cin, userInput);

	if(userInput == "interactive"){
		return interactiveMode();
	}
	else if(userInput == "file") {
		cout << "Filename: ";
		getline(cin, userInput);
		generateReportFromFile(userInput);
	}

	return 0;
}

int interactiveMode(){
	FormulaHasher f;

	string last = "";
	string userInput = "";
	
	do {
		cout << "Input for Processing: ";
		getline(cin, userInput);

		if (userInput == "") {
			break;
		}

		string hash = f.hash(userInput);

		cout << "> ";

		for (char c : hash) {
			cout << (int)c << " ";
		}
		cout << "\n";

		int equalityCount = 0;
		if (last != "") {
			for (unsigned int i = 0; i < hash.length(); i++) {
				equalityCount += hash[i] == last[i];
			}

			cout << ((double)(equalityCount) / (double)(hash.length()) * 100.0) << "% match\n";
		}
		last = hash;
	} while (userInput != "end" && userInput != "exit");

	return 0;
}
