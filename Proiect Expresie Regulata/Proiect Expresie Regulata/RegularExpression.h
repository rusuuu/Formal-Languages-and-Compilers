#pragma once

#include "NondeterministicFiniteAutomatonWithLambdaTransitions.h"

#include <iostream>
#include <string>
#include <stack>

class RegularExpression
{
public:

	RegularExpression(std::string Infixed, std::string Postfixed);
	RegularExpression();

	void InfixedToPostfixed();
	NondeterministicFiniteAutomatonWithLambdaTransitions PosfixedToNondeterministicFiniteAutomatonWithLambdaTransitions();

	void Output();

private:

	std::string m_Infixed;
	std::string m_Postfixed;
};


