#pragma once

#include <vector>
#include <unordered_map>
#include <cctype>
#include <string>
#include <sstream>

class Grammar
{
public:

	Grammar(std::string startSymbol, std::string Vn, std::string Vt, std::unordered_map<std::string, std::string> Rules);
	Grammar();

	void PrintRules();
	bool IsValidGrammar();
	void ReadGrammar(const std::string& filename);
	bool IsRegular();
	bool IsTerminalString(const std::string& inputString);
	std::string GenerateWord();
	bool IsNonTerminal(const char character);
	//bool operator==(const std::string& other);

	std::string GetVN();
	std::string GetVT();
	std::string GetStartSymbol();
	std::unordered_map<std::string, std::string> GetPRules();

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

//S
//S B C
//a b c
//S->aSBC
//S->aBC
//CB->BC
//aB->ab
//bB->bb
//cB->Bc
//cC->cc
//bC->bc

//S
//S A B C D
//a b
//S->aA
//A->bB
//B->aC
//C->bD
//D->aS