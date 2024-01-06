#pragma once

#include <vector>
#include <map>
#include <cctype>
#include <string>
#include <sstream>
#include <random>
#include <ctime>
#include <set>

class Grammar
{
public:

	Grammar(std::string startSymbol, std::string Vn, std::string Vt, std::multimap<std::string, std::string> Rules, int zCounter);
	Grammar();

	bool IsValidGrammar();
	void DisplayGrammar(const std::string& filename);
	void PrintGrammar();
	void ReadGrammar(const std::string& filename);
	bool IsRegular();
	bool IsTerminalString(const std::string& inputString);
	std::string GenerateWord();

	bool IsContextFree();
	bool IsRightHandSideValid(const std::string& rhs);

	void SimplifyGrammar();
	void RemoveUnproductiveSymbols();
	void RemoveUnreachableSymbols();
	void RemoveNullProductions();
	void RemoveUnitProductions();
	bool IsNonTerminal(const char character);

	void ConvertToGNF();
	void ConvertToGNFwithPrint();
	bool IsStartSymbolOnRHS();
	void RemoveStartSymbolFromRHS();
	void ConvertToCNF();
	void ReplaceTerminals(std::string& rhs);
	void BreakDownLongerProductions(const std::string& lhs, const std::string& rhs, std::multimap<std::string, std::string>& newProductions);
	std::string GenerateUniqueNonTerminal();
	void EliminateImmediateLeftRecursion();
	bool IsTerminal(const std::string& symbol);
	void ApplyLemma1(std::string lhs, std::string rhs, std::multimap<std::string, std::string>& newPRules);
	void ApplyLemma2(std::string lhs, std::string rhs, std::multimap<std::string, std::string>& newPRules);
	void ConvertRulesToGNF();
	
	

	std::string GetVN();
	std::string GetVT();
	std::string GetStartSymbol();
	std::multimap<std::string, std::string> GetPRules();

private:
	std::string m_VN;
	std::string m_VT;
	std::string m_startSymbol;
	std::multimap<std::string, std::string> m_PRules;
	static std::mt19937 rng;

	int m_zCounter;

	
	std::string Trim(const std::string& str)
	{
		auto start = str.find_first_not_of(" \t");
		auto end = str.find_last_not_of(" \t");
		return start == std::string::npos ? "" : str.substr(start, end - start + 1);
	}

	std::set<std::string> FindNullableNonTerminals();
	std::set<std::string> GenerateCombinations(const std::string& production, const std::set<std::string>& nullableNonTerminals);
};


