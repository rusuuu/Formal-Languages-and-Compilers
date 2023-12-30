#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <tuple>

class NondeterministicFiniteAutomatonWithLambdaTransitions
{
public:

	NondeterministicFiniteAutomatonWithLambdaTransitions(std::vector<std::string> Q, std::string sigma, std::string q0, std::vector<std::string> F, std::vector<std::tuple<std::string, char, std::vector<std::string>>> delta);
	NondeterministicFiniteAutomatonWithLambdaTransitions(const NondeterministicFiniteAutomatonWithLambdaTransitions& automaton);
	NondeterministicFiniteAutomatonWithLambdaTransitions();

	void PrintAutomaton();

	void set_Q(std::vector<std::string> Q);
	void set_sigma(std::string sigma);
	void set_q0(std::string q0);
	void set_F(std::vector<std::string> F);
	void set_delta(std::vector<std::tuple<std::string, char, std::vector<std::string>>> delta);

	std::vector<std::string> get_Q();
	std::string get_sigma();
	std::string get_q0();
	std::vector<std::string> get_F();
	std::vector<std::tuple<std::string, char, std::vector<std::string>>> get_delta();

private:

	std::vector<std::string> m_Q;
	std::string m_sigma;
	std::string m_q0;
	std::vector<std::string> m_F;
	std::vector<std::tuple<std::string, char, std::vector<std::string>>> m_delta;
};

