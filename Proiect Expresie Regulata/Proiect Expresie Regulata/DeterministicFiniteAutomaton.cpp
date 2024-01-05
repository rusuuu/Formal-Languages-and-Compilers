#include "DeterministicFiniteAutomaton.h"
#include <fstream>

#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
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
	// Define the output file
	std::ofstream outFile("dfa_output.txt");

	// Check if the file is open
	if (!outFile.is_open()) 
	{
		std::cerr << "Failed to open file for writing.\n";
		return;
	}

	// Helper lambda to write to both console and file
	auto write = [&](const std::string& text) 
		{
		std::cout << text;
		outFile << text;
		};

	write("States: ");
	if (m_Q.empty()) 
	{
		write("No states are defined.\n");
	}
	else
	{
		for (const auto& state : m_Q)
		{
			write(state + " ");
		}
		write("\n");
	}

	write("Alphabet: " + (m_sigma.empty() ? "No alphabet defined.\n" : m_sigma + "\n"));
	write("Start State: " + (m_q0.empty() ? "No start state defined.\n" : m_q0 + "\n"));

	write("Final States: ");
	if (m_F.empty()) 
	{
		write("No final states defined.\n");
	}
	else {
		for (const auto& finalState : m_F) {
			write(finalState + " ");
		}
		write("\n");
	}

	write("Transitions:\n");
	if (m_delta.empty()) {
		write("No transitions defined.\n");
	}
	else {
		for (const auto& transition : m_delta) {
			std::string transitionString = "delta(" + std::get<0>(transition) + ", " + std::get<1>(transition) + ") -> ";
			for (const auto& nextState : std::get<2>(transition)) {
				transitionString += nextState + " ";
			}
			write(transitionString + "\n");
		}
	}

	// Close the file
	outFile.close();
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

void DeterministicFiniteAutomaton::setAutomaton(NondeterministicFiniteAutomatonWithLambdaTransitions automaton)
{
	m_automaton = automaton;
}

NondeterministicFiniteAutomatonWithLambdaTransitions DeterministicFiniteAutomaton::getAutomaton()
{
	return m_automaton;
}

void DeterministicFiniteAutomaton::lambdaClosure(std::vector<std::string> &oldStates, int &stateContor, std::tuple<std::string, char, std::vector<std::string>> newTransition, std::vector<std::string> &Q)
{
	std::queue<std::string> states;
	std::vector<std::string> lambdaClosure;

	for(std::string state: oldStates)
	{
		states.push(state);
		lambdaClosure.push_back(state);
	}

	while(!states.empty())
	{
		std::string state = states.front();
		for (auto& transition : m_automaton.get_delta())
		{
			if (std::get<0>(transition) == state && std::get<1>(transition) == '\0')
			{
				for (auto& finalState : std::get<2>(transition))
				{
					states.push(finalState);
					lambdaClosure.push_back(finalState);
				}
				break;
			}
		}
		states.pop();
	}

	bool found = false;

	for (const auto& closures : m_lambdaClosures)
	{
		if (lambdaClosure == std::get<1>(closures))
		{
			std::get<2>(newTransition) = { std::get<2>(closures) };
			newTransitions.push_back(newTransition);
			found = true;
			break;
		}
	}

	if (!found)
	{
		std::string newState = "q" + std::to_string(stateContor);
		stateContor++;

		newStates.push(newState);
		m_lambdaClosures.push_back({ oldStates, lambdaClosure, newState });
		std::get<2>(newTransition) = { newState };
		newTransitions.push_back(newTransition);
		Q.push_back(newState);
	}
}

void DeterministicFiniteAutomaton::newTransition(std::string &startState, int& stateContor, std::vector<std::string>& Q)
{
	std::tuple<std::string, char, std::vector<std::string>> newTransition;
	std::get<0>(newTransition) = startState;

	std::vector<std::string> oldStates;
	for (auto& lambdaClosure : m_lambdaClosures)
	{
		if(std::get<2>(lambdaClosure)==startState)
		{
			oldStates = std::get<1>(lambdaClosure);
			break;
		}
	}

	for (char letter : m_automaton.get_sigma())
	{
		std::vector<std::string> finalState;
		for (std::string state : oldStates)
		{
			for (auto& transition : m_automaton.get_delta())
			{

				if (std::get<0>(transition) == state && std::get<1>(transition) == letter)
				{
					finalState.push_back(std::get<2>(transition)[0]);
					std::get<1>(newTransition) = letter;
					break;
				}
			}
		}

		if (!finalState.empty()) lambdaClosure(finalState, stateContor, newTransition, Q);
	}
}

DeterministicFiniteAutomaton DeterministicFiniteAutomaton::NondeterministicFiniteAutomatonWithLambdaTransitionsToDeteministicFiniteAutomaton()
{
	std::vector<std::string> Q;
	std::string sigma;
	std::string q0;
	std::vector<std::string> F;
	std::vector<std::tuple<std::string, char, std::vector<std::string>>> delta;

	sigma = m_automaton.get_sigma();
	std::string currentState = m_automaton.get_q0();
	int stateContor = 0;

	std::queue<std::string> states;
	std::vector<std::string> lambdaClosure;

	states.push(currentState);
	lambdaClosure.push_back(currentState);

	while (!states.empty())
	{
		std::string state = states.front();
		for (auto& transition : m_automaton.get_delta())
		{
			if (std::get<0>(transition) == state && std::get<1>(transition) == '\0')
			{
				for (auto& finalState : std::get<2>(transition))
				{
					states.push(finalState);
					lambdaClosure.push_back(finalState);
				}
				break;
			}
		}
		states.pop();
	}

	std::string newState = "q" + std::to_string(stateContor);
	stateContor++;

	newStates.push(newState);
	m_lambdaClosures.push_back({ {currentState}, lambdaClosure, newState });
	Q.push_back(newState);

	while(!newStates.empty())
	{
		newTransition(newStates.front(), stateContor, Q);
		newStates.pop();
	}

	delta = newTransitions;

	q0 = Q[0];
	F = { Q[Q.size() - 1] };

	//std::cout << std::endl;
	//std::cout << "DFA: " << std::endl;
	DeterministicFiniteAutomaton DFA(Q, sigma, q0, F, delta);
	//DFA.PrintAutomaton();

	return DFA;
}
