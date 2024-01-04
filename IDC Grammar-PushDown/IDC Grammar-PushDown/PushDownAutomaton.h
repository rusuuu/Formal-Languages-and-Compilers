#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <queue>
#include <tuple>

class PushDownAutomaton
{
public:

	PushDownAutomaton(std::vector<std::string> Q, std::string sigma, std::string gamma, std::string q0, std::string Z0, std::vector<std::string> F, std::vector<std::tuple<std::string, char, std::string, std::vector<std::pair<std::string, std::vector<std::string>>>>> delta);
	PushDownAutomaton();

	void PrintAutomaton();
	bool CheckWord(std::string word);
	bool isDeterministic();

private:

	std::vector<std::string> m_Q;
	std::string m_sigma;
	std::string m_gamma;
	std::string m_q0;
	std::string m_Z0;
	std::vector<std::string> m_F;
	std::vector<std::tuple<std::string, char, std::string, std::vector<std::pair<std::string, std::vector<std::string>>>>> m_delta;
	std::stack<std::string> m_pushDownMemory;
};

