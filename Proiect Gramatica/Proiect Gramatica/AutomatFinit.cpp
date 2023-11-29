#include "AutomatFinit.h"

#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

bool ElementIsInQ(std::vector<std::string> m_Q, std::string element)
{
	if (std::find(m_Q.begin(), m_Q.end(), element) == m_Q.end())
		return false;
	return true;
}

bool AutomatFinit::VerifyAutomaton()
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


	int index = 0;
	bool valid = true;
	while (index < m_delta.size() && valid)
	{
		if (std::get<0>(m_delta[index]) == m_q0)
			valid = false;
		index++;
	}
	if (valid)
		return false;

	return true;

}

void AutomatFinit::ReadAutomaton()
{
	std::ifstream input("AutomatonInput.txt");

	if (!input.is_open())
	{
		std::cerr << "Unable to open file\n";
		return;
	}

	std::string line;

	// Read Q
	//to be continued
}

void AutomatFinit::PrintAutomaton()
{
	//to be continued
}

bool AutomatFinit::CheckWord(std::string word)
{
	if (word.size() < 1) return false;

	//to be continued
}

bool AutomatFinit::IsDeterministic()
{
	int i = 0;

	for (int index = 0; index < m_delta.size(); index++)
	{
		if (std::get<2>(m_delta[index]).size() > 1) return true;

	}
	return false;
}
