#pragma once
#pragma once

#include <vector>
#include <string>
#include <tuple>
#include <fstream>
#include <cctype>
#include <sstream>

#include "Grammar.h"

class FiniteAutomaton
{
public:

	FiniteAutomaton(std::vector<std::string> Q, std::string sigma, std::string q0, std::vector<std::string> F, std::vector<std::tuple<std::string, char, std::vector<std::string>>> delta);
	FiniteAutomaton();

	bool VerifyAutomaton();
	void ReadAutomaton(const std::string& filename);
	void PrintAutomaton();
	bool CheckWord(std::string word);
	bool IsDeterministic();
	FiniteAutomaton GrammarToFiniteAutomaton(Grammar grammar);

private:

	std::vector<std::string> m_Q; //multime nevida de elemente finite numite stari
	std::string m_sigma; //alfabet de intrare (multime nevida)
	std::string m_q0; //stare initiala
	std::vector<std::string> m_F; //multimea starilor finale
	std::vector<std::tuple<std::string, char, std::vector<std::string>>> m_delta; //tranzitiile

	// Utility function to trim spaces from a string
	std::string Trim(const std::string& str)
	{
		auto start = str.find_first_not_of(" \t");
		auto end = str.find_last_not_of(" \t");
		return start == std::string::npos ? "" : str.substr(start, end - start + 1);
	}

	// Utility function to split a string by spaces
	std::vector<std::string> Split(const std::string& str)
	{
		std::istringstream iss(str);
		std::vector<std::string> tokens;
		std::string token;
		while (iss >> token)
		{
			tokens.push_back(token);
		}
		return tokens;
	}
};

//nedeterminist
//q0 q1 q2 q3
//01
//q0
//q2
//q0 0 q1
//q0 1 q0 q1
//q1 0 \0
//q1 1 q3
//q2 0 q2
//q2 1 q3
//q3 0 q2
//q3 1 q1 q2

//nedeterminist complex
//q0 q1 q2 q3 q4
//01
//q0
//q2 q4
//q0 0 q0 q3
//q0 1 q0 q1
//q1 0 \0
//q1 1 q2
//q2 0 q2
//q2 1 q2
//q3 0 q4
//q3 1 \0
//q4 0 q4
//q4 1 q4

//determinist
//q0 q1
//ab
//q0
//q1
//q0 a q0
//q0 b q1
//q1 a q1
//q1 b q0class FiniteAutomaton


