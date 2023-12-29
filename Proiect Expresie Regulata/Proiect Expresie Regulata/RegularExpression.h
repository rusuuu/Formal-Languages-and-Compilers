#pragma once

#include <iostream>
#include <string>
#include <stack>

class RegularExpression
{
public:

	RegularExpression(std::string Infixed, std::string Postfixed);
	RegularExpression();

	void InfixedToPostfixed();
	void Output();

private:

	std::string m_Infixed;
	std::string m_Postfixed;
};


