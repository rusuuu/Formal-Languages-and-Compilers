#include "RegularExpression.h"

int precedence(char operand)
{
	switch (operand)
	{
		case'*':
		{
			return 3;
		}
		case'.':
		{
			return 2;
		}
		case'|':
		{
			return 1;
		}
		case')':
		{
			return -1;
		}
		case'(':
		{
			return -2;
		}
		default:
		{
			return 0;
		}
	}
}

RegularExpression::RegularExpression(std::string Infixed, std::string Postfixed):
	m_Infixed(std::move(Infixed)),
	m_Postfixed(std::move(Postfixed))
{
}

RegularExpression::RegularExpression()
{
	m_Infixed = "";
	m_Postfixed = "";
}

void RegularExpression::InfixedToPostfixed()
{
	std::stack<char> operators;

	for(char character: m_Infixed)
	{
		if (precedence(character) == 0) m_Postfixed.push_back(character);

		if (precedence(character) == -2) operators.push('(');

		if(precedence(character)==-1)
		{
			while(precedence(operators.top())!=-2)
			{
				m_Postfixed.push_back(operators.top());
				operators.pop();
			}
			operators.pop();
		}

		if (precedence(character) > 0)
		{
			while (!operators.empty() && precedence(character) <= precedence(operators.top()))
			{
				m_Postfixed.push_back(operators.top());
				operators.pop();
			}
			operators.push(character);
		}
	}

	while (!operators.empty())
	{
		m_Postfixed.push_back(operators.top());
		operators.pop();
	}
}

void RegularExpression::OrOperator(int &stateContor)
{
	NondeterministicFiniteAutomatonWithLambdaTransitions Automaton3;
	NondeterministicFiniteAutomatonWithLambdaTransitions Automaton2 = AutomatonStack.top(); AutomatonStack.pop();
	NondeterministicFiniteAutomatonWithLambdaTransitions Automaton1 = AutomatonStack.top(); AutomatonStack.pop();

	//setting Automaton3's m_q0
	Automaton3.set_q0(Automaton1.get_q0());

	//increasing Automaton1 and Automaton2's m_q0 and Automaton1's m_F by 1 to facilitate the later addition of the new transitions 
	std::string newState = Automaton1.get_q0();
	std::string numPart = newState.substr(1);
	newState = newState[0];

	int num = std::stoi(numPart);
	num++;

	newState = newState + std::to_string(num);
	Automaton1.set_q0(newState); //setting Automaton1's m_q0;

	newState = Automaton2.get_q0();
	numPart = newState.substr(1);
	newState = newState[0];

	num = std::stoi(numPart);
	num++;

	newState = newState + std::to_string(num);
	Automaton2.set_q0(newState); //setting Automaton2's m_q0;

	newState = Automaton1.get_F()[0];
	numPart = newState.substr(1);
	newState = newState[0];

	num = std::stoi(numPart);
	num++;

	newState = newState + std::to_string(num);
	Automaton1.set_F({ newState }); //setting Automaton2's m_F

	//creating Automaton3's m_Q by adding Automaton1 and Automaton2's m_Q and the 2 new states
	std::vector<std::string> Automaton3_Q;

	for (const std::string& state : Automaton1.get_Q())
	{
		Automaton3_Q.push_back(state);
	}
	for (const std::string& state : Automaton2.get_Q())
	{
		Automaton3_Q.push_back(state);
	}

	newState = Automaton2.get_F()[0];
	numPart = newState.substr(1);
	newState = newState[0];

	num = std::stoi(numPart);
	num++;

	newState = newState + std::to_string(num);

	Automaton3_Q.push_back(newState);
	Automaton2.set_F({ newState }); //setting Automaton2's m_F

	numPart = newState.substr(1);
	newState = newState[0];

	num = std::stoi(numPart);
	num++;

	newState = newState + std::to_string(num);
	Automaton3_Q.push_back(newState);
	Automaton3.set_F({ newState }); //setting Automaton3's m_F

	Automaton3.set_Q(Automaton3_Q);

	//creating Automaton3's m_sigma
	std::string Automaton3_sigma;
	Automaton3_sigma = Automaton1.get_sigma();
	for (char c : Automaton2.get_sigma())
	{
		if (Automaton1.get_sigma().find(c) == std::string::npos)
		{
			Automaton3_sigma += c;
		}
	}
	Automaton3.set_sigma(Automaton3_sigma);

	//increasing the number of every state from Automaton1 and Automaton2's m_delta by 1 to add the new starting and ending states
	std::vector<std::tuple<std::string, char, std::vector<std::string>>> Automaton1_delta = Automaton1.get_delta();
	for (auto& transition : Automaton1_delta)
	{
		std::string& state = std::get<0>(transition);
		std::string numPart = state.substr(1);
		state = state[0];

		int num = std::stoi(numPart);
		num++;

		state = state + std::to_string(num);

		std::vector<std::string>& states = std::get<2>(transition);
		for (std::string& state : states)
		{
			std::string numPart = state.substr(1);
			state = state[0];

			int num = std::stoi(numPart);
			num++;

			state = state + std::to_string(num);
		}
	}
	Automaton1.set_delta(Automaton1_delta);

	std::vector<std::tuple<std::string, char, std::vector<std::string>>> Automaton2_delta = Automaton2.get_delta();
	for (auto& transition : Automaton2_delta)
	{
		std::string& state = std::get<0>(transition);
		std::string numPart = state.substr(1);
		state = state[0];

		int num = std::stoi(numPart);
		num++;

		state = state + std::to_string(num);

		std::vector<std::string>& states = std::get<2>(transition);
		for (std::string& state : states)
		{
			std::string numPart = state.substr(1);
			state = state[0];

			int num = std::stoi(numPart);
			num++;

			state = state + std::to_string(num);
		}
	}
	Automaton2.set_delta(Automaton2_delta);

	//adding the transitions form Automaton1 and Automaton2 in Automaton3
	std::vector<std::tuple<std::string, char, std::vector<std::string>>> Automaton3_delta;
	for (const auto& transition : Automaton1_delta)
	{
		Automaton3_delta.push_back(transition);
	}
	for (const auto& transition : Automaton2_delta)
	{
		Automaton3_delta.push_back(transition);
	}

	//adding the new transitions in Automaton3
	std::string emptyString = "";
	Automaton3_delta.push_back({ Automaton3.get_q0(), emptyString[0], {Automaton1.get_q0(),Automaton2.get_q0()} });
	Automaton3_delta.push_back({ Automaton1.get_F()[0], emptyString[0], {Automaton3.get_F()[0]} });
	Automaton3_delta.push_back({ Automaton2.get_F()[0], emptyString[0], {Automaton3.get_F()[0]} });

	Automaton3.set_delta(Automaton3_delta);

	stateContor += 2;
	AutomatonStack.push(Automaton3);
	std::cout << std::endl;
	Automaton3.PrintAutomaton();
}

void RegularExpression::ConcatenationOperator(int &stateContor)
{
	NondeterministicFiniteAutomatonWithLambdaTransitions Automaton3;
	NondeterministicFiniteAutomatonWithLambdaTransitions Automaton2 = AutomatonStack.top(); AutomatonStack.pop();
	NondeterministicFiniteAutomatonWithLambdaTransitions Automaton1 = AutomatonStack.top(); AutomatonStack.pop();

	//decreasing Automaton2's m_F
	std::string newState = Automaton2.get_F()[0];
	std::string numPart = newState.substr(1);
	newState = newState[0];

	int num = std::stoi(numPart);
	num--;

	newState = newState + std::to_string(num);
	Automaton2.set_F({ newState }); //setting Automaton2's m_F;

	//setting Automaton3's m_q0
	Automaton3.set_q0(Automaton1.get_q0());

	//setting Automaton3's m_F
	Automaton3.set_F(Automaton2.get_F());

	//creating Automaton3's m_Q by adding Automaton1 and Automaton2's m_Q and eliminating 1 state
	std::vector<std::string> Automaton3_Q;

	for (const std::string& state : Automaton1.get_Q())
	{
		Automaton3_Q.push_back(state);
	}
	for (const std::string& state : Automaton2.get_Q())
	{
		Automaton3_Q.push_back(state);
	}
	Automaton3_Q.pop_back();

	Automaton3.set_Q(Automaton3_Q);

	//creating Automaton3's m_sigma
	std::string Automaton3_sigma;
	Automaton3_sigma = Automaton1.get_sigma();
	for (char c : Automaton2.get_sigma())
	{
		if (Automaton1.get_sigma().find(c) == std::string::npos)
		{
			Automaton3_sigma += c;
		}
	}
	Automaton3.set_sigma(Automaton3_sigma);

	//decreasing the number of every state from Automaton2's m_delta by 1
	std::vector<std::tuple<std::string, char, std::vector<std::string>>> Automaton1_delta = Automaton1.get_delta();
	std::vector<std::tuple<std::string, char, std::vector<std::string>>> Automaton2_delta = Automaton2.get_delta();
	for (auto& transition : Automaton2_delta)
	{
		std::string& state = std::get<0>(transition);
		std::string numPart = state.substr(1);
		state = state[0];

		int num = std::stoi(numPart);
		num--;

		state = state + std::to_string(num);

		std::vector<std::string>& states = std::get<2>(transition);
		for (std::string& state : states)
		{
			std::string numPart = state.substr(1);
			state = state[0];

			int num = std::stoi(numPart);
			num--;

			state = state + std::to_string(num);
		}
	}
	Automaton2.set_delta(Automaton2_delta);

	//adding the transitions form Automaton1 and Automaton2 in Automaton3
	std::vector<std::tuple<std::string, char, std::vector<std::string>>> Automaton3_delta;
	for (const auto& transition : Automaton1_delta)
	{
		Automaton3_delta.push_back(transition);
	}
	for (const auto& transition : Automaton2_delta)
	{
		Automaton3_delta.push_back(transition);
	}
	Automaton3.set_delta(Automaton3_delta);

	stateContor--;
	AutomatonStack.push(Automaton3);
	std::cout << std::endl;
	Automaton3.PrintAutomaton();
}

void RegularExpression::KleinStarOperator(int &stateContor)
{
	NondeterministicFiniteAutomatonWithLambdaTransitions Automaton = AutomatonStack.top(); AutomatonStack.pop();

	//variables that facilitate the addition of transitions
	std::string newState = Automaton.get_q0();
	std::string numPart = newState.substr(1);
	newState = newState[0];

	int num = std::stoi(numPart);
	num++;

	newState = newState + std::to_string(num);

	std::string oldInitialState = newState;
	std::string oldFinalState;

	//creating Automaton3's m_Q by adding Automaton1 and Automaton2's m_Q and the 2 new states
	std::vector<std::string> Automaton_Q = Automaton.get_Q();

	newState = Automaton.get_F()[0];
	numPart = newState.substr(1);
	newState = newState[0];

	num = std::stoi(numPart);
	num++;

	newState = newState + std::to_string(num);

	Automaton_Q.push_back(newState);
	oldFinalState = newState;

	numPart = newState.substr(1);
	newState = newState[0];

	num = std::stoi(numPart);
	num++;

	newState = newState + std::to_string(num);
	Automaton_Q.push_back(newState);
	Automaton.set_F({ newState }); //setting Automaton's m_F

	Automaton.set_Q(Automaton_Q);

	//increasing the number of every state from Automaton's m_delta by 1
	std::vector<std::tuple<std::string, char, std::vector<std::string>>> Automaton_delta = Automaton.get_delta();
	for (auto& transition : Automaton_delta)
	{
		std::string& state = std::get<0>(transition);
		std::string numPart = state.substr(1);
		state = state[0];

		int num = std::stoi(numPart);
		num++;

		state = state + std::to_string(num);

		std::vector<std::string>& states = std::get<2>(transition);
		for (std::string& state : states)
		{
			std::string numPart = state.substr(1);
			state = state[0];

			int num = std::stoi(numPart);
			num++;

			state = state + std::to_string(num);
		}
	}

	//adding the new transitions in Automaton
	std::string emptyString = "";
	Automaton_delta.push_back({ Automaton.get_q0(), emptyString[0], {Automaton.get_F()[0],oldInitialState } });
	Automaton_delta.push_back({ oldFinalState, emptyString[0], {oldInitialState, Automaton.get_F()[0]} });
	Automaton.set_delta(Automaton_delta);

	stateContor += 2;
	AutomatonStack.push(Automaton);
	std::cout << std::endl;
	Automaton.PrintAutomaton();
}

void RegularExpression::AutomateConstruction(int &stateContor, char &character)
{
	NondeterministicFiniteAutomatonWithLambdaTransitions Automaton;

	//creating Automaton's m_Q, m_q0 and m_F
	std::vector<std::string> Automaton_Q;

	std::string newState = "q" + std::to_string(stateContor);
	Automaton_Q.push_back(newState);
	Automaton.set_q0(newState);

	std::string numPart = newState.substr(1);
	newState = newState[0];

	int num = std::stoi(numPart);
	num++;

	newState = newState + std::to_string(num);
	Automaton_Q.push_back(newState);

	Automaton.set_F({ newState });
	Automaton.set_Q(Automaton_Q);

	//creating Automaton's m_sigma
	Automaton.set_sigma({ character });

	//creating Automaton's m_delta
	std::vector<std::tuple<std::string, char, std::vector<std::string>>> Automaton_delta;
	Automaton_delta.push_back({ Automaton.get_q0(), character, Automaton.get_F() });
	Automaton.set_delta(Automaton_delta);

	stateContor += 2;
	AutomatonStack.push(Automaton);
	std::cout << std::endl;
	Automaton.PrintAutomaton();
}

NondeterministicFiniteAutomatonWithLambdaTransitions RegularExpression::PosfixedToNondeterministicFiniteAutomatonWithLambdaTransitions()
{
	int stateContor = 0;

	for(char character : m_Postfixed)
	{
		if (character == '|')
		{
			OrOperator(stateContor);
		}
		else if (character == '.')
		{
			ConcatenationOperator(stateContor);
		}
		else if(character=='*')
		{
			KleinStarOperator(stateContor);
		}
		else
		{
			AutomateConstruction(stateContor, character);
		}
	}

	return AutomatonStack.top();
}

void RegularExpression::Output()
{
	std::cout << "Infixed Form: " << m_Infixed << std::endl;
	std::cout << "Postfixed Form: " << m_Postfixed << std::endl;
	std::cout << std::endl;
}
