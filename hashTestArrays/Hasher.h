#pragma once

#include <string>
#include <array>

using std::string;
using std::array;

namespace ctl_f {
	/*
		Virtual interface for any class that will take an input string and return a hashed output
	*/
	template<unsigned int t> class Hasher {
	public:
		virtual array<char, t> hash(string input) = 0;
	};
};

