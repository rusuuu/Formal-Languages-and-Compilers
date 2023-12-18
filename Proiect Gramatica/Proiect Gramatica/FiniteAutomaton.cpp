#include "FiniteAutomaton.h"

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

FiniteAutomaton::FiniteAutomaton(std::vector<std::string> Q, std::string sigma, std::string q0,
	std::vector<std::string> F, std::vector<std::tuple<std::string, char, std::vector<std::string>>> delta):
	m_Q(std::move(Q)),
	m_sigma(std::move(sigma)),
	m_q0(std::move(q0)),
	m_F(std::move(F)),
	m_delta(std::move(delta))
{
}

FiniteAutomaton::FiniteAutomaton()
{
}

bool FiniteAutomaton::VerifyAutomaton()
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

void FiniteAutomaton::ReadAutomaton(const std::string& filename)
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

void FiniteAutomaton::PrintAutomaton()
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

int FiniteAutomaton::CurrentStateToIndex(std::string &currentState, std::string &word)
{
	for (int index = 0; index < m_delta.size(); index++)
	{
		if (std::get<0>(m_delta[index]) == currentState && std::get<1>(m_delta[index]) == word[0])
		{
			return index;
		}
	}
	return -1;
}

std::string FiniteAutomaton::IndexToCurrentState(int index)
{
	for (int Index = 0; Index < m_delta.size(); Index++)
	{
		if (index == Index)
			return std::get<0>(m_delta[index]);
	}

	return std::string();
}

bool FiniteAutomaton::CheckWord(std::string word)
{
	if (word.size() < 1) return false;

	std::string currentState = m_q0;
	//std::queue<std::tuple<std::string, std::string>> statesQueue;
	std::queue<std::tuple<int, std::string>> statesQueue;
	//statesQueue.push(std::make_tuple(currentState, word));
	statesQueue.push(std::make_tuple(CurrentStateToIndex(currentState, word), word));

	std::vector<std::string> futureStates;
	while (!statesQueue.empty())
	{
		//std::tuple<std::string, std::string> currentFront = statesQueue.front();
		std::tuple<int, std::string> currentFront = statesQueue.front();
		std::string currentWord = std::get<1>(currentFront);
		char letter = std::get<1>(currentFront)[0];
		currentState = IndexToCurrentState(std::get<0>(currentFront));

		auto iterator = std::find(m_F.begin(), m_F.end(), currentState);

		if (iterator != m_F.end())
			return true;

		if (currentWord.size() > 0)
		{
			for (int index = 0; index < m_delta.size(); index++)
			{
				if (std::get<0>(m_delta[index]) == currentState && std::get<1>(m_delta[index]) == letter)
				{
					const auto& nextStates = std::get<2>(m_delta[index]);

					if (nextStates.size() > 0)
					{
						futureStates = nextStates;
						break;
					}
				}
			}

			for (auto state : futureStates)
			{
				statesQueue.push(std::make_tuple(CurrentStateToIndex(state, currentWord), currentWord.substr(1)));
			}
		}
		statesQueue.pop();
	}

	return false;
}

bool FiniteAutomaton::IsDeterministic()
{
	for (int index = 0; index < m_delta.size(); index++)
		if (std::get<2>(m_delta[index]).size() > 1) return false;

	return true;
}

FiniteAutomaton FiniteAutomaton::GrammarToFiniteAutomaton(Grammar grammar)
{
	std::string VN = grammar.GetVN();
	std::string VT = grammar.GetVT();
	std::string startSymbol = grammar.GetStartSymbol();
	std::multimap<std::string, std::string> PRules = grammar.GetPRules();

	std::vector<std::string> Q;
	std::string sigma;
	std::string q0;
	std::vector<std::string> F;
	std::vector<std::tuple<std::string, char, std::vector<std::string>>> delta;
	std::string T("T");

	q0 = startSymbol;

	for(char letter: VN)
	{
		if(letter!=' ') Q.push_back(std::string(1, letter));
	}

	for (char letter : VT)
	{
		if (letter != ' ') sigma.push_back(letter);
	}

	for(const auto& production: PRules)
	{
		std::string firstPart = production.first;
		const auto& secondPart = production.second;
		std::vector<std::string> thirdElement;

		if (secondPart.size() < 1)
		{
			if (std::find(F.begin(), F.end(), firstPart) == F.end()) F.push_back(firstPart);
			thirdElement.push_back(firstPart);
		}
		else
		{
			if (secondPart.size() >= 2)
				thirdElement.push_back(std::string(1, secondPart[1]));
			else
				thirdElement.push_back(T);
		}

		auto it = std::find_if(delta.begin(), delta.end(),
			[&](const auto& tuple)
			{
				return std::get<0>(tuple) == firstPart && std::get<1>(tuple) == secondPart[0];
			});

		if (it != delta.end())
		{
			std::get<2>(*it).insert(std::end(std::get<2>(*it)), std::begin(thirdElement), std::end(thirdElement));
		}
		else
		{
			delta.push_back(std::make_tuple(firstPart, secondPart[0], thirdElement));
		}
	}

	for (const auto& transition : delta)
	{
		std::string state = std::get<0>(transition);
		char symbol = std::get<1>(transition);
		std::vector<std::string> next_states = std::get<2>(transition);

		if (std::find(next_states.begin(), next_states.end(), state) != next_states.end()) 
		{
			if (std::find(F.begin(), F.end(), T) == F.end()) F.push_back(T);
		}
	}


	FiniteAutomaton automaton(Q, sigma, q0, F, delta);
	return automaton;
}
