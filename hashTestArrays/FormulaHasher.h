#pragma once
#include <string>
#include <array>

#include "Hasher.h"

using std::string;
using std::array;

#define HASH_SIZE 512U

namespace ctl_f {

	class FormulaHasher : public Hasher<HASH_SIZE> {
	public:
		FormulaHasher();
		~FormulaHasher();

		//static const unsigned int HASH_SIZE = 512U;

		array<char, HASH_SIZE> hash(string input) override;

	private:
		// will be called if given input is greater than the hash_size
		array<char, HASH_SIZE> shrinkInput(string input);

		// will be called if the given input is less than the hash_size
		array<char, HASH_SIZE> growInput(string input);

		// fillter each character across the string
		array<char, HASH_SIZE> xorFilter(const array<char, HASH_SIZE>& input);
		string xorFilter(string input);

		// will be used to "stitch" or "shuffle" the input around
		array<char, HASH_SIZE> sewInput(const array<char, HASH_SIZE>& input);

		// the base hashing function
		array<char, HASH_SIZE> _hash_core(string input);
	};
}

