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
	// Check if F is empty
	if (m_F.size() == 0)
		return false;

	// Check if initial state is an element of Q
	if (ElementIsInQ(m_Q, m_q0) == false)
		return false;

	//Check if characters from sigma are elements of Q
	for (int index = 0; index < m_sigma.length(); index++)
	{
		std::string elements;
		elements = elements + m_sigma[index];
		if (ElementIsInQ(m_Q, elements)) return false;
	}

	//Check if elements from delta are formed from characters that are elements of sigma and Q
	for (int index1 = 0; index1 < m_delta.size(); index1++)
	{
		if (!ElementIsInQ(m_Q, std::get<0>(m_delta[index1]))) return false;

		if (m_sigma.find(std::get<1>(m_delta[index1])) == -1) return false;

		for (int index2 = 0; index2 < std::get<2>(m_delta[index1]).size(); index2++)
			if (!ElementIsInQ(m_Q, std::get<2>(m_delta[index1])[index2])) return false;
	}

	//Check if at least a transition from delta has q0 as an initial state
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

	// Read states
	if (std::getline(file, line))
	{
		m_Q = Split(Trim(line));
		std::cout << "States: ";
		for (const auto& state : m_Q)
		{
			std::cout << state << " ";
		}
		std::cout << "\n";
	}

	// Read input alphabet
	if (std::getline(file, line))
	{
		m_sigma = Trim(line);
		std::cout << "Input Alphabet: " << m_sigma << "\n";
	}

	// Read initial state
	if (std::getline(file, line))
	{
		m_q0 = Trim(line);
		std::cout << "Initial State: " << m_q0 << "\n";
	}

	// Read final states
	if (std::getline(file, line))
	{
		m_F = Split(Trim(line));
		std::cout << "Final States: ";
		for (const auto& finalState : m_F)
		{
			std::cout << finalState << " ";
		}
		std::cout << "\n";
	}

	// Read transitions
	std::cout << "Transitions:\n";
	while (std::getline(file, line))
	{
		line = Trim(line);
		std::vector<std::string> tokens = Split(line);

		if (tokens.size() >= 3)
		{
			std::string state = tokens[0];
			char symbol = tokens[1][0];
			std::vector<std::string> nextStates;

			// Handle "\0" as an empty string
			for (size_t i = 2; i < tokens.size(); ++i)
			{
				if (tokens[i] != "\\0")
				{
					nextStates.push_back(tokens[i]);
				}
			}

			m_delta.emplace_back(state, symbol, nextStates);

			std::cout << "delta( " << state << ", " << symbol << " ) = { ";
			for (const auto& nextState : nextStates)
			{
				std::cout << nextState << " ";
			}
			std::cout << "}";
			std::cout << "\n";
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

//bool FiniteAutomaton::OldCheckWord(std::string word)
//{
//	if (word.size() < 1) return false;
//
//	std::string currentState = m_q0;
//
//	if(IsDeterministic())
//	{
//		for (char letter : word)
//		{
//			if (m_sigma.find(letter) == -1) return false;
//
//			for (int index = 0; index < m_delta.size(); index++)
//			{
//				if (std::get<0>(m_delta[index]) == currentState && std::get<1>(m_delta[index]) == letter)
//				{
//					const auto& nextStates = std::get<2>(m_delta[index]);
//
//					if (nextStates.size() > 0) 
//						currentState = nextStates[0];
//					else 
//						return false;
//
//					break;
//				}
//			}
//		}
//
//		auto iterator = std::find(m_F.begin(), m_F.end(), currentState);
//
//		if (iterator != m_F.end()) 
//			return true;
//		else 
//			return false;
//	}
//	else
//	{
//		std::queue<std::tuple<std::string, std::string>> statesQueue;
//		statesQueue.push(std::make_tuple(currentState, word));
//
//		std::vector<std::string> futureStates;
//		while(!statesQueue.empty())
//		{
//			std::tuple<std::string, std::string> currentFront = statesQueue.front();
//			std::string currentWord = std::get<1>(currentFront);
//			char letter = std::get<1>(currentFront)[0];
//			currentState = std::get<0>(currentFront);
//
//			auto iterator = std::find(m_F.begin(), m_F.end(), currentState);
//
//			if (iterator != m_F.end())
//				return true;
//
//			if (currentWord.size() > 1)
//			{
//				for (int index = 0; index < m_delta.size(); index++)
//				{
//					if (std::get<0>(m_delta[index]) == currentState && std::get<1>(m_delta[index]) == letter)
//					{
//						const auto& nextStates = std::get<2>(m_delta[index]);
//
//						if (nextStates.size() > 0)
//						{
//							futureStates = nextStates;
//							break;
//						}
//					}
//				}
//
//				for (auto state : futureStates)
//				{
//					statesQueue.push(std::make_tuple(state, currentWord.substr(1)));
//				}
//			}
//			statesQueue.pop();
//		}
//
//		return false;
//	}
//}

bool FiniteAutomaton::CheckWord(std::string word)
{
	if (word.size() < 1) return false;

	std::string currentState = m_q0;
	std::queue<std::tuple<std::string, std::string>> statesQueue;
	statesQueue.push(std::make_tuple(currentState, word));

	std::vector<std::string> futureStates;
	while (!statesQueue.empty())
	{
		std::tuple<std::string, std::string> currentFront = statesQueue.front();
		std::string currentWord = std::get<1>(currentFront);
		char letter = std::get<1>(currentFront)[0];
		currentState = std::get<0>(currentFront);

		auto iterator = std::find(m_F.begin(), m_F.end(), currentState);

		if (iterator != m_F.end())
			return true;

		if (currentWord.size() > 1)
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
				statesQueue.push(std::make_tuple(state, currentWord.substr(1)));
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
	//Create grammar components
	std::string VN = grammar.GetVN();
	std::string VT = grammar.GetVT();
	std::string startSymbol = grammar.GetStartSymbol();
	std::multimap<std::string, std::string> PRules = grammar.GetPRules();

	//Create automaton components
	std::vector<std::string> Q;
	std::string sigma;
	std::string q0;
	std::vector<std::string> F;
	std::vector<std::tuple<std::string, char, std::vector<std::string>>> delta;

	//S -> m_q0
	q0 = startSymbol;

	//Create states (VN -> Q)
	for(char letter: VN)
	{
		if(letter!=' ') Q.push_back(std::string(1, letter));
	}

	//Create alphabet (VT -> sigma)
	for (char letter : VT)
	{
		if (letter != ' ') sigma.push_back(letter);
	}

	//Create transitions P -> delta
	for(const auto& production: PRules)
	{
		std::string firstPart = production.first;
		const auto& secondPart = production.second;

		if (secondPart.size() <= 1) //For productions that contain lambda
		{
			// Check if firstPart is not already in m_F
			if (std::find(F.begin(), F.end(), firstPart) == F.end()) F.push_back(firstPart);
		}
		else //For productions that don't contain lambda
		{
			std::vector<std::string> thirdElement;
			if (secondPart.size() >= 2)
				thirdElement.push_back(std::string(1, secondPart[1]));
			else
				thirdElement.push_back("");


			// Search for an existing tuple with the same firstPart and secondPart[0]
			auto it = std::find_if(delta.begin(), delta.end(),
				[&](const auto& tuple)
				{
					return std::get<0>(tuple) == firstPart && std::get<1>(tuple) == secondPart[0];
				});

			if (it != delta.end())
			{
				//There is already a transition that contains the same first element and path so we only add the second element next to the existing ones if it's the case
				std::get<2>(*it).insert(std::end(std::get<2>(*it)), std::begin(thirdElement), std::end(thirdElement));
			}
			else
			{
				//There isn't any transition that contains the same first element and path so we make a new one
				delta.push_back(std::make_tuple(firstPart, secondPart[0], thirdElement));
			}
		}
	}

	FiniteAutomaton automaton(Q, sigma, q0, F, delta);
	return automaton;
}
