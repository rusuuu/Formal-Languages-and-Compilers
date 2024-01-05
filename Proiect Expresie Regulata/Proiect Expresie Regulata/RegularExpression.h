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

	void OrOperator(int& stateContor);
	void ConcatenationOperator(int& stateContor);
	void KleinStarOperator(int& stateContor);
	void AutomateConstruction(int& stateContor, char& character);
	NondeterministicFiniteAutomatonWithLambdaTransitions PosfixedToNondeterministicFiniteAutomatonWithLambdaTransitions();

	void Output();

	bool VerifyExpression() const;


private:

	std::string m_Infixed;
	std::string m_Postfixed;
	std::stack<NondeterministicFiniteAutomatonWithLambdaTransitions> AutomatonStack;
};


