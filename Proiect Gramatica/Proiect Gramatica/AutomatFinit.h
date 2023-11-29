#pragma once

#include <vector>
#include <string>
#include <tuple>
#include <fstream>
#include <cctype>
#include <sstream>

class AutomatFinit
{
public:

	bool VerifyAutomaton();
	void ReadAutomaton();
	void PrintAutomaton();
	bool CheckWord(std::string word);
	bool IsDeterministic();

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

