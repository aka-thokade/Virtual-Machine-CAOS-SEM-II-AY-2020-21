#pragma once
#include <string>
#include <array>
#include <iostream>
#include <vector>

class Register {
	public:
		int IR[4], M[2], IC[2], R[4], C;
		std::string buffer;
		std::vector<std::string> instruction;
		std::vector<std::string> ins_word;
		std::vector<std::string> memory;
};