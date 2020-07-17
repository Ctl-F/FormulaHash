/*
 * Auther: Ctl-F (Spencer Brough)
 * Date: 07/15/2020
 */

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
	return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

double matchStrings(const string& first, const string& second){
	int matchedChars = 0;
	int maxIndex = (first.length() < second.length()) ? first.length() : second.length();
	
	if(maxIndex == 0){
		cout << "Error! Got zero!!" << "\n";
		return 0.0;
	}
	
	for(int i=0; i<maxIndex; i++){
		matchedChars += first[i] == second[i];
	}
	cout << matchedChars << " / " << maxIndex << " are the same\n";
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

void generateReportFromFile(const string& fname){
	ifstream f(fname);
	vector<string> lines;
	string line;

	if(f.is_open()){
		while(f.good()){
			getline(f, line);
			lines.push_back(line);
		}
		f.close();
	}
	cout << "Read " << lines.size() << " lines from file\n";
	
	vector<string> hashes;
	FormulaHasher hasher;
	for(string s : lines){
		hashes.push_back(hasher.hash(s));
	}

	cout << "Hashing finished, beginning comparison\n";
	string hashA, hashB;
	double avg = 0.0, max = -100.0, min = 200.0;
	int count = 0;
	int targetCount = addtorial(hashes.size());
	long timeStamp = currentTime();
	for(int i=0; i<hashes.size(); i++){
		hashA = hashes[i];
		for(int j=0; j<hashes.size(); j++){
			++count;
			if(i == j) {
				continue;
			}
			hashB = hashes[j];
			double match = matchStrings(hashA, hashB);
			
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
	avg /= (double)hashes.size();
	cout << "Average: " << avg << "\nMax: " << max << " - Min: " << min << "\n";
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
			for (int i = 0; i < hash.length(); i++) {
				equalityCount += hash[i] == last[i];
			}

			cout << ((double)(equalityCount) / (double)(hash.length()) * 100.0) << "% match\n";
		}
		last = hash;
	} while (userInput != "end" && userInput != "exit");

	return 0;
}
