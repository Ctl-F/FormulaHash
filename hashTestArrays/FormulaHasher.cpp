#include "FormulaHasher.h"

namespace ctl_f {

	FormulaHasher::FormulaHasher() {
		
	}

	FormulaHasher::~FormulaHasher() {

	}

	array<char, HASH_SIZE> FormulaHasher::hash(string input) {
		array<char, HASH_SIZE> hash1 = _hash_core(input);
		array<char, HASH_SIZE> hash2 = xorFilter(sewInput(xorFilter(hash1)));

		for (unsigned int i = 0; i < HASH_SIZE; i++) {
			hash1[i] = (hash1[i] + ~hash2[i]) ^ (hash2[i] - ~hash1[i]);
		}
		return hash1;
	}


	array<char, HASH_SIZE> FormulaHasher::_hash_core(string input) {
		array<char, HASH_SIZE> output;
		if (input.length() < HASH_SIZE) {
			output = growInput(input);
		}
		else if (input.length() > HASH_SIZE) {
			output = shrinkInput(input);
		}

		return sewInput(output);
	}

	/*
		Take the character from the end, split it in half
		and add it to the front of the string's characters
		and so on
	*/
	array<char, HASH_SIZE> FormulaHasher::shrinkInput(string input) {
		if (input == "") {
			array<char, HASH_SIZE> e;
			return e;
		}

		input = xorFilter(input);

		int index = 0;
		char currentChar;
		while (input.length() > HASH_SIZE) {
			currentChar = input[input.length()-1];

			input[index] += (currentChar & 0b11110000) >> 4;
			input[index] ^= (~(currentChar & 0b00001111)) | (currentChar & 0b11110000);

			if (++index >= input.length()) {
				index = 0;
			}

			input[index] += (currentChar & 0b00001111);
			input[index] ^= (~((currentChar & 0b11110000) >> 4)) | ((currentChar & 0b00001111) << 4);

			if (++index >= input.length()) {
				index = 0;
			}

			input = input.erase(input.length() - 1);
		}

		array<char, HASH_SIZE> output;
		for (int i = 0; i < HASH_SIZE; i++) {
			output[i] = input[i];
		}
		return xorFilter(output);
	}

	// apply a simple xor filter over the string by itself
	// use an accumulated shift to offset the character each time
	string FormulaHasher::xorFilter(string input) {
		if (input == "") {
			return "";
		}

		char c;
		int accumulator = 0;
		for (int i = 0; i < input.length(); i++) {
			c = input[i];
			for (int j = 0; j < input.length(); j++) {
				input[j] ^= (c << accumulator);
				if (++accumulator > 4) {
					accumulator = 0;
				}
			}
		}
		return input;
	}

	// apply a simple xor filter over the string by itself
	// use an accumulated shift to offset the character each time
	array<char, HASH_SIZE> FormulaHasher::xorFilter(const array<char, HASH_SIZE>& input) {
		char c;
		int accumulator = 0;
		for (int i = 0; i < input.size(); i++) {
			c = input[i];
			for (int j = 0; j < input.size(); j++) {
				input[j] ^= (c << accumulator);
				if (++accumulator > 4) {
					accumulator = 0;
				}
			}
		}
		return input;
	}

	/*
		We are going to take each bit in each character
		invert it and move forward by an accumulated counter
		e.g.
		given: 01000010

		we will grab the zero, invert it to get a 1
		then we add 1 to the accumulator
		then we move forward by the accumulator (1) and grab the 1
		inverting it we get 0 and the output character becomes 10
		and we add 2 to the accumulator (3)
		we then move forward by 3 getting a 0, inverting it becomes a 1
		and adding it to the output become 101
		then we add 1 to the accumulator (4) and we move forward by 4
		and so on.

		NOTE: for a zero bit we add 1 to the accumulator
			  for a one bit we add 2 to the accumulator

		When the accumulator reaches 8 we will wrap it down to zero and
		continue until we've generated a new byte. then we move forward to
		the next byte and repeat until the given string is the correct length
	*/
	string FormulaHasher::growInput(string input) {
		int length = input.length();
		if (length < 1) {
			return "";
		}

		// with strings less than 4 characters long this seems to break it
		// at this stage so just hold off for now
		if (length >= 4) {
			input = xorFilter(input);
		}

		const unsigned char BIT_MASK = 0b00000001;
		unsigned int currentBit = 0;

		// as explained above
		int accumulator = 0;
		
		// the byte of data we are currently generating
		unsigned char currentGeneration = 0;

		// how many bits have we generated so far in the current generation
		int generationCount = 0;

		// Take the first character, swap the halves and modulate by the length to get the starting index
		// current index in the input string
		unsigned char firstChar = input[0];

		size_t currentStringIndex = ((firstChar & 0b11110000) >> 4) | ((firstChar & 0b00001111) << 4);
		currentStringIndex %= input.length();

		// reference to the original string character
		char currentStringCharacter = input[currentStringIndex];

		while (input.length() < HASH_SIZE) {
			// take the current bit
			currentBit = (currentStringCharacter >> accumulator) & BIT_MASK;
			
			// logical not to get just a zero or a one
			// a bitwise not would give us a 255 or a 254
			currentGeneration |= !currentBit;
			currentGeneration <<= 1;

			accumulator += currentBit + 1;
			if (accumulator > 7) {
				accumulator -= 8;
			}

			if (++generationCount == 8) {
				generationCount = 0;
				input += (char)currentGeneration;
				if (++currentStringIndex >= input.length()) {
					currentStringIndex = 0;

				}
				currentStringCharacter = input[currentStringIndex];
			}
		}

		return xorFilter(input);
	}

	/*
	 * Take the character, apply it to a character with an accumulative offset over the string
	 * to apply: Add, mod by 7, xor by result
	 * */
	string FormulaHasher::sewInput(string input) {
		if(input == "") {
			return "";
		}
		if(input.length() < 2){
			return input;
		}

		input = xorFilter(input);

		int accumulatedOffset = 1;
		char currentChar = 0;
		for(int i=0; i<input.length(); i++){
			currentChar = input[i];
			input[i + accumulatedOffset] ^= (currentChar + input[i + accumulatedOffset]) % 7;
			if(++accumulatedOffset + i >= input.length()){
				accumulatedOffset = -i;
			}
		}

		return xorFilter(input);
	}

};
