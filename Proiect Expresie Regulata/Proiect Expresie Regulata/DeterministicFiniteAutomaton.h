#pragma once

#include "NondeterministicFiniteAutomatonWithLambdaTransitions.h"

#include <vector>
#include <string>
#include <tuple>
#include <queue>
#include <fstream>
#include <cctype>
#include <sstream>

class DeterministicFiniteAutomaton
{
public:

	DeterministicFiniteAutomaton(std::vector<std::string> Q, std::string sigma, std::string q0, std::vector<std::string> F, std::vector<std::tuple<std::string, char, std::vector<std::string>>> delta);
	DeterministicFiniteAutomaton();

	bool VerifyAutomaton();
	void ReadAutomaton(const std::string& filename);
	void PrintAutomaton();
	bool CheckWord(std::string word);
	bool IsDeterministic();

	void setAutomaton(NondeterministicFiniteAutomatonWithLambdaTransitions automaton);
	NondeterministicFiniteAutomatonWithLambdaTransitions getAutomaton();

	void lambdaClosure(std::vector<std::string>& oldStates, int& stateContor, std::tuple<std::string, char, std::vector<std::string>> newTransition, std::vector<std::string>& Q);
	void newTransition(std::string &startState, int& stateContor, std::vector<std::string>& Q);

	DeterministicFiniteAutomaton NondeterministicFiniteAutomatonWithLambdaTransitionsToDeteministicFiniteAutomaton();

private:

	std::vector<std::string> m_Q;
	std::string m_sigma;
	std::string m_q0;
	std::vector<std::string> m_F;
	std::vector<std::tuple<std::string, char, std::vector<std::string>>> m_delta;

	NondeterministicFiniteAutomatonWithLambdaTransitions m_automaton;

	std::vector<std::tuple<std::vector<std::string>, std::vector<std::string>, std::string>> m_lambdaClosures;
	std::queue<std::string> newStates;
	std::vector<std::tuple<std::string, char, std::vector<std::string>>> newTransitions;

	std::string Trim(const std::string& str)
	{
		auto start = str.find_first_not_of(" \t");
		auto end = str.find_last_not_of(" \t");
		return start == std::string::npos ? "" : str.substr(start, end - start + 1);
	}

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