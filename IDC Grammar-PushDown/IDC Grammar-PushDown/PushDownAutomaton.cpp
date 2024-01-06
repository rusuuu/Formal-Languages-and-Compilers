#include "PushDownAutomaton.h"

PushDownAutomaton::PushDownAutomaton(std::vector<std::string> Q, std::string sigma, std::string gamma, std::string q0, std::string Z0, std::vector<std::string> F, std::vector<std::tuple<std::string, char, std::string, std::vector<std::pair<std::string, std::vector<std::string>>>>> delta) :
	m_Q(Q),
	m_sigma(sigma),
	m_gamma(gamma),
	m_q0(q0),
	m_Z0(Z0),
	m_F(F),
	m_delta(delta)
{
}

PushDownAutomaton::PushDownAutomaton()
{
}

void PushDownAutomaton::PrintAutomaton()
{
	std::cout << "States: ";
	for (const auto& state : m_Q)
	{
		std::cout << state << " ";
	}
	std::cout << "\n";

	std::cout << "Input Alphabet: " << m_sigma << "\n";

	std::cout << "Push Down Memory Alphabet: " << m_gamma << "\n";

	std::cout << "Initial State: " << m_q0 << "\n";

	std::cout << "Start Symbol for Push Down Memory: " << m_Z0 << "\n";

	std::cout << "Final States: ";
	for (const auto& finalState : m_F)
	{
		std::cout << finalState << " ";
	}
	std::cout << "\n";

	std::cout << "Transitions:\n";
	for (const auto& transition : m_delta)
	{
		std::cout << "delta( " << std::get<0>(transition) << ", " << std::get<1>(transition) << ", " << std::get<2>(transition) << " ) = { ";
		for (const auto& pushDownElements : std::get<3>(transition))
		{
			std::cout << "(" << pushDownElements.first << " , ";
			for (const auto& str : pushDownElements.second)
			{
				std::cout << str;
			}
			std::cout << ") ";
		}
		std::cout << "}\n";
	}
}

bool PushDownAutomaton::CheckWord(std::string word)
{
	if (word.size() < 1) return false;

	std::string currentState = m_q0;
	std::stack<std::string> pushDownMemory;
	pushDownMemory.push(m_Z0);
	std::queue<std::tuple<std::string, std::string, std::stack<std::string>>> statesQueue;
	statesQueue.push(std::make_tuple(currentState, word, pushDownMemory));

	std::vector<std::pair<std::string, std::vector<std::string>>> futureStates;
	while (!statesQueue.empty())
	{
		std::tuple<std::string, std::string, std::stack<std::string>> currentFront = statesQueue.front();

		currentState = std::get<0>(currentFront);
		std::string currentWord = std::get<1>(currentFront);
		char letter = std::get<1>(currentFront)[0];
		std::stack<std::string> currentStack = std::get<2>(currentFront);

		std::queue<std::tuple<std::string, std::string, std::stack<std::string>>> tempQueue = statesQueue;
		while (!tempQueue.empty())
		{
			std::cout << "CURRENT STATE: " << std::get<0>(tempQueue.front()) << "\nCURRENT WORD: " << std::get<1>(tempQueue.front()) << "\nCURRENT STACK: ";
			while (!std::get<2>(tempQueue.front()).empty())
			{
				std::cout << std::get<2>(tempQueue.front()).top() << " ";
				std::get<2>(tempQueue.front()).pop();
			}
			tempQueue.pop();
		}
		std::cout << std::endl;

		if (currentWord.size() >= 0)
		{
			for (int index = 0; index < m_delta.size(); index++)
			{
				std::cout << std::get<0>(m_delta[index]) << " = " << currentState << " " << std::get<1>(m_delta[index]) << " = " << letter << " " << std::get<2>(m_delta[index]) << " = " << std::get<2>(currentFront).top() << std::endl;
				if (std::get<0>(m_delta[index]) == currentState && std::get<1>(m_delta[index]) == letter && std::get<2>(m_delta[index]) == std::get<2>(currentFront).top())
				{
					const auto& nextStates = std::get<3>(m_delta[index]);

					if (nextStates.size() > 0)
					{
						futureStates = nextStates;
						break;
					}
				}
			}

			for (auto pairInfo : futureStates)
			{
				std::stack<std::string> futureStack = currentStack;

				futureStack.pop();
				if (!pairInfo.second.empty())
				{
					if (pairInfo.second.size() > 1)
						futureStack.push(pairInfo.second[1]);
					futureStack.push(pairInfo.second[0]);
				}

				std::cout << "FUTURE STACK: ";
				auto tempStack = futureStack;
				while (!tempStack.empty())
				{
					std::cout << tempStack.top() << " ";
					tempStack.pop();
				}
				std::cout << std::endl;
				std::cout << std::endl;

				if (currentWord.size() == 0)
				{
					auto iterator = std::find(m_F.begin(), m_F.end(), pairInfo.first);

					if (iterator != m_F.end() && futureStack.top() == m_Z0)
						return true;
				}
				else statesQueue.push(std::make_tuple(pairInfo.first, currentWord.substr(1), futureStack));
			}
		}
		statesQueue.pop();
	}

	return false;
}

bool PushDownAutomaton::isDeterministic()
{
	for (int index = 0; index < m_delta.size(); index++)
		if (std::get<3>(m_delta[index]).size() > 1) return false;

	return true;
}

PushDownAutomaton PushDownAutomaton::IDCtoPDAconversion(Grammar grammar)
{
	grammar.ConvertToGNF();
	std::vector<std::string> Q = { "q0" };
	std::string sigma = grammar.GetVT();
	std::string gamma = grammar.GetVN();
	std::string q0 = "q0";
	std::string Z0 = grammar.GetStartSymbol();
	std::vector<std::string> F = {};
	std::vector<std::tuple<std::string, char, std::string, std::vector<std::pair<std::string, std::vector<std::string>>>>> delta;

	auto PRules = grammar.GetPRules(); 

	for (const auto& rule : PRules)
	{
		std::string fromState = "q0";
		char inputSymbol = rule.first[0];
		std::string toState = "q0";
		std::vector<std::pair<std::string, std::vector<std::string>>> stackOperations;

		for (char stackSymbol : rule.second)
		{
			std::string stackOperation(1, stackSymbol);
			stackOperations.push_back(std::make_pair(toState, std::vector<std::string>{stackOperation}));
		}

		delta.push_back(std::make_tuple(fromState, inputSymbol, toState, stackOperations));
	}

	return PushDownAutomaton(Q, sigma, gamma, q0, Z0, F, delta);

}
