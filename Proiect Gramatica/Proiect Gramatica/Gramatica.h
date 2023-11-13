#pragma once
#include <vector>
#include <unordered_map>
#include <string>
class Gramatica
{
public:
	bool IsValidGrammar();
	void ReadGrammar(const std::string& filename);
	Gramatica(std::string startSymbol, std::string Vn, std::string Vt, std::unordered_map<std::string, std::string> Rules);
	bool IsRegular();
	bool IsTerminalString(const std::string& inputString);
	bool IsNonTerminal(const char character);
	bool operator==(const std::string& other);
private:
	std::string m_VN;
	std::string m_VT;
	std::string m_startSymbol;
	std::unordered_map<std::string, std::string> m_PRules;


};

