/*
 * Auther: Ctl-F (Spencer Brough)
 * Date: 07/15/2020
 */
#pragma once

#include <string>

using std::string;

/*
 * Basic interface for any hashing class
 */
namespace ctl_f {
	/*
		Virtual interface for any class that will take an input string and return a hashed output
	*/
	class Hasher {
	public:
		virtual string hash(string input) = 0;
	};
};

