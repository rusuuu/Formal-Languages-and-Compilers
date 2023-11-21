#pragma once
#include <vector>
#include <unordered_map>
#include <cctype>
#include <string>
#include <sstream>

class Gramatica
{
public:
	bool IsValidGrammar();
	void ReadGrammar(const std::string& filename);
	Gramatica(std::string startSymbol, std::string Vn, std::string Vt, std::unordered_map<std::string, std::string> Rules);
	Gramatica();
	bool IsRegular();
	bool IsTerminalString(const std::string& inputString);
	bool IsNonTerminal(const char character);
	//bool operator==(const std::string& other);

private:
	std::string m_VN;
	std::string m_VT;
	std::string m_startSymbol;
	std::unordered_map<std::string, std::string> m_PRules;

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

