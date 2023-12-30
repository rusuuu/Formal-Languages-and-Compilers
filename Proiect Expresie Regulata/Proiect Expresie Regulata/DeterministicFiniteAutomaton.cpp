#include "DeterministicFiniteAutomaton.h"

#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <tuple>

bool ElementIsInQ(std::vector<std::string> m_Q, std::string element)
{
	if (std::find(m_Q.begin(), m_Q.end(), element) == m_Q.end()) return false;

	return true;
}

DeterministicFiniteAutomaton::DeterministicFiniteAutomaton(std::vector<std::string> Q, std::string sigma, std::string q0,
	std::vector<std::string> F, std::vector<std::tuple<std::string, char, std::vector<std::string>>> delta) :
	m_Q(std::move(Q)),
	m_sigma(std::move(sigma)),
	m_q0(std::move(q0)),
	m_F(std::move(F)),
	m_delta(std::move(delta))
{
}

DeterministicFiniteAutomaton::DeterministicFiniteAutomaton()
{
}

bool DeterministicFiniteAutomaton::VerifyAutomaton()
{
	if (m_Q.size() == 0)
		return false;

	if (m_sigma.size() == 0)
		return false;

	if (m_q0.size() == 0)
		return false;

	if (m_F.size() == 0)
		return false;

	if (ElementIsInQ(m_Q, m_q0) == false)
		return false;

	for (int index = 0; index < m_sigma.length(); index++)
	{
		std::string elements;
		elements = elements + m_sigma[index];
		if (ElementIsInQ(m_Q, elements)) return false;
	}

	for (int index1 = 0; index1 < m_delta.size(); index1++)
	{
		if (!ElementIsInQ(m_Q, std::get<0>(m_delta[index1]))) return false;

		if (m_sigma.find(std::get<1>(m_delta[index1])) == -1) return false;

		for (int index2 = 0; index2 < std::get<2>(m_delta[index1]).size(); index2++)
			if (!ElementIsInQ(m_Q, std::get<2>(m_delta[index1])[index2])) return false;
	}

	int index = 0;
	bool valid = false;
	while (index < m_delta.size())
	{
		if (std::get<0>(m_delta[index]) == m_q0)
		{
			valid = true;
			break;
		}
		index++;
	}
	if (!valid)
		return false;

	return true;

}

void DeterministicFiniteAutomaton::ReadAutomaton(const std::string& filename)
{
	std::ifstream file(filename);
	if (!file.is_open())
	{
		std::cerr << "Unable to open file\n";
		return;
	}

	std::string line;

	if (std::getline(file, line))
	{
		m_Q = Split(Trim(line));
	}

	if (std::getline(file, line))
	{
		m_sigma = Trim(line);
	}

	if (std::getline(file, line))
	{
		m_q0 = Trim(line);
	}

	if (std::getline(file, line))
	{
		m_F = Split(Trim(line));
	}

	while (std::getline(file, line))
	{
		line = Trim(line);
		std::vector<std::string> tokens = Split(line);

		if (tokens.size() >= 3)
		{
			std::string state = tokens[0];
			char symbol = tokens[1][0];
			std::vector<std::string> nextStates;

			for (size_t i = 2; i < tokens.size(); ++i)
			{
				if (tokens[i] != "\\0")
				{
					nextStates.push_back(tokens[i]);
				}
			}

			m_delta.emplace_back(state, symbol, nextStates);


		}
	}

	file.close();
}

void DeterministicFiniteAutomaton::PrintAutomaton()
{
	std::cout << "States: ";
	for (const auto& state : m_Q)
	{
		std::cout << state << " ";
	}
	std::cout << "\n";

	std::cout << "Input Alphabet: " << m_sigma << "\n";

	std::cout << "Initial State: " << m_q0 << "\n";

	std::cout << "Final States: ";
	for (const auto& finalState : m_F)
	{
		std::cout << finalState << " ";
	}
	std::cout << "\n";

	std::cout << "Transitions:\n";
	for (const auto& transition : m_delta)
	{
		std::cout << "delta( " << std::get<0>(transition) << ", " << std::get<1>(transition) << " ) = { ";
		for (const auto& nextState : std::get<2>(transition))
		{
			std::cout << nextState << " ";
		}
		std::cout << "}\n";
	}
}

bool DeterministicFiniteAutomaton::CheckWord(std::string word)
{
	if (word.size() < 1) return false;

	std::string currentState = m_q0;
	std::string currentWord = word;

	for (char letter : word)
	{
		if (m_sigma.find(letter) == -1) return false;

		for (int index = 0; index < m_delta.size(); index++)
		{
			if (std::get<0>(m_delta[index]) == currentState && std::get<1>(m_delta[index]) == letter)
			{
				const auto& nextStates = std::get<2>(m_delta[index]);

				if (nextStates.size() > 0)
					currentState = nextStates[0];
				else
					return false;

				break;
			}
		}
	}

	auto iterator = std::find(m_F.begin(), m_F.end(), currentState);

	if (iterator != m_F.end())
		return true;
	else
		return false;
}

bool DeterministicFiniteAutomaton::IsDeterministic()
{
	for (int index = 0; index < m_delta.size(); index++)
		if (std::get<2>(m_delta[index]).size() > 1) return false;

	return true;
}

DeterministicFiniteAutomaton DeterministicFiniteAutomaton::NondeterministicFiniteAutomatonWithLambdaTransitionsToDeteministicFiniteAutomaton()
{
	DeterministicFiniteAutomaton DFA;



	return DFA;
}
