#pragma once
#include <string>
#include <array>
#include <iostream>
#include <vector>

class Register {
	public:
		char IR[4], R[4];
		int M[2], IC[2], C;
		char buffer[40];
		std::vector<std::string> instruction;
		std::vector<std::string> ins_word;
};