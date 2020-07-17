/*
 * Author: Ctl-F (Spencer Brough)
 * Date: 07/15/2020
 */

#pragma once
#include <string>

#include "Hasher.h"

using std::string;

namespace ctl_f {

	class FormulaHasher : public Hasher {
	public:
		FormulaHasher();
		~FormulaHasher();

		string hash(string input) override;

	private:
		// will be called if given input is greater than the hash_size
		string shrinkInput(string input);

		// will be called if the given input is less than the hash_size
		string growInput(string input);

		// fillter each character across the string
		string xorFilter(string input);

		// will be used to "stitch" or "shuffle" the input around
		string sewInput(string input);

		const unsigned int HASH_SIZE = 512;
	};
}

