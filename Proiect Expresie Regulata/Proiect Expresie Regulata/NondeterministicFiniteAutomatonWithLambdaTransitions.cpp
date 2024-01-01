#include "NondeterministicFiniteAutomatonWithLambdaTransitions.h"

NondeterministicFiniteAutomatonWithLambdaTransitions::NondeterministicFiniteAutomatonWithLambdaTransitions(std::vector<std::string> Q, std::string sigma, std::string q0, std::vector<std::string> F, std::vector<std::tuple<std::string, char, std::vector<std::string>>> delta) :
	m_Q(std::move(Q)),
	m_sigma(std::move(sigma)),
	m_q0(std::move(q0)),
	m_F(std::move(F)),
	m_delta(std::move(delta))
{
}

NondeterministicFiniteAutomatonWithLambdaTransitions::NondeterministicFiniteAutomatonWithLambdaTransitions(const NondeterministicFiniteAutomatonWithLambdaTransitions& automaton) :
	m_Q(automaton.m_Q),
	m_sigma(automaton.m_sigma),
	m_q0(automaton.m_q0),
	m_F(automaton.m_F),
	m_delta(automaton.m_delta)
{
}

NondeterministicFiniteAutomatonWithLambdaTransitions::NondeterministicFiniteAutomatonWithLambdaTransitions()
{
}

void NondeterministicFiniteAutomatonWithLambdaTransitions::PrintAutomaton()
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

void NondeterministicFiniteAutomatonWithLambdaTransitions::set_Q(std::vector<std::string> Q)
{
	m_Q = Q;
}

void NondeterministicFiniteAutomatonWithLambdaTransitions::set_sigma(std::string sigma)
{
	m_sigma = sigma;
}

void NondeterministicFiniteAutomatonWithLambdaTransitions::set_q0(std::string q0)
{
	m_q0 = q0;
}

void NondeterministicFiniteAutomatonWithLambdaTransitions::set_F(std::vector<std::string> F)
{
	m_F = F;
}

void NondeterministicFiniteAutomatonWithLambdaTransitions::set_delta(std::vector<std::tuple<std::string, char, std::vector<std::string>>> delta)
{
	m_delta = delta;
}

std::vector<std::string> NondeterministicFiniteAutomatonWithLambdaTransitions::get_Q()
{
	return m_Q;
}

std::string NondeterministicFiniteAutomatonWithLambdaTransitions::get_sigma()
{
	return m_sigma;
}

std::string NondeterministicFiniteAutomatonWithLambdaTransitions::get_q0()
{
	return m_q0;
}

std::vector<std::string> NondeterministicFiniteAutomatonWithLambdaTransitions::get_F()
{
	return m_F;
}

std::vector<std::tuple<std::string, char, std::vector<std::string>>> NondeterministicFiniteAutomatonWithLambdaTransitions::get_delta()
{
	return m_delta;
}

NondeterministicFiniteAutomatonWithLambdaTransitions& NondeterministicFiniteAutomatonWithLambdaTransitions::operator=(const NondeterministicFiniteAutomatonWithLambdaTransitions& automaton)
{
	if (this != &automaton)
	{
		m_Q = automaton.m_Q;
		m_sigma = automaton.m_sigma;
		m_q0 = automaton.m_q0;
		m_F = automaton.m_F;
		m_delta = automaton.m_delta;
	}

	return *this;
}
