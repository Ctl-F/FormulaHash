#pragma once

#include <string>

using std::string;

namespace ctl_f {
	/*
		Virtual interface for any class that will take an input string and return a hashed output
	*/
	class Hasher {
	public:
		virtual string hash(string input) = 0;
	};
};

