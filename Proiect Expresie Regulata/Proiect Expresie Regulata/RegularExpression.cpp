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

NondeterministicFiniteAutomatonWithLambdaTransitions RegularExpression::PosfixedToNondeterministicFiniteAutomatonWithLambdaTransitions()
{
	std::stack<NondeterministicFiniteAutomatonWithLambdaTransitions> AutomatonStack;
	int stateContor = 0;

	for(char character : m_Postfixed)
	{
		if (character == '|')
		{
			NondeterministicFiniteAutomatonWithLambdaTransitions Automaton3;
			NondeterministicFiniteAutomatonWithLambdaTransitions Automaton2 = AutomatonStack.top(); AutomatonStack.pop();
			NondeterministicFiniteAutomatonWithLambdaTransitions Automaton1 = AutomatonStack.top(); AutomatonStack.pop();

			//increasing the number of every state from Automaton1 and Automaton2's m_Q by 1 to add the new starting and ending states
			std::vector<std::string> Automaton1_Q = Automaton1.get_Q();
			for (std::string& state : Automaton1_Q)
			{
				std::string numPart = state.substr(1);
				state = state[0];

				int num = std::stoi(numPart);
				num++;

				state = state + std::to_string(num);
			}
			Automaton1.set_Q(Automaton1_Q);

			std::vector<std::string> Automaton2_Q = Automaton2.get_Q();
			for (std::string& state : Automaton2_Q)
			{
				std::string numPart = state.substr(1);
				state = state[0];

				int num = std::stoi(numPart);
				num++;

				state = state + std::to_string(num);
			}

			//increasing the number of Automaton2's initial state(m_q0)
			std::string initialState = Automaton2.get_q0();
			std::string numPart = initialState.substr(1);
			initialState = initialState[0];

			int num = std::stoi(numPart);
			num += 1;

			initialState = initialState + std::to_string(num);

			Automaton2.set_q0(initialState);

			//increasing the number of Automaton1's final state(m_F)
			std::vector<std::string> Automaton1_F;
			std::string finalState = Automaton1.get_F()[0];
			numPart = finalState.substr(1);
			finalState = finalState[0];

			num = std::stoi(numPart);
			num += 1;

			finalState = finalState + std::to_string(num);

			Automaton1_F.push_back(finalState);
			Automaton1.set_F(Automaton1_F);

			//adding initial state to Automaton3's m_q0
			std::string Automaton3_q0 = Automaton3.get_q0();
			Automaton3_q0 = Automaton1.get_q0();
			Automaton3.set_q0(Automaton3_q0);

			//increasing the number of Automaton1's initial state(m_q0)
			initialState = Automaton1.get_q0();
			numPart = initialState.substr(1);
			initialState = initialState[0];

			num = std::stoi(numPart);
			num += 1;

			initialState = initialState + std::to_string(num);

			Automaton1.set_q0(initialState);

			//creating Automaton3's final state by increasing the number of Automaton2's final state by 2
			std::vector<std::string> Automaton2_F;
			finalState = Automaton2.get_F()[0];
			numPart = finalState.substr(1);
			finalState = finalState[0];

			num = std::stoi(numPart);
			num+=1;

			finalState = finalState + std::to_string(num);

			Automaton2_F.push_back(finalState);
			Automaton2.set_F(Automaton2_F);

			numPart = finalState.substr(1);
			finalState = finalState[0];

			num = std::stoi(numPart);
			num += 1;

			finalState = finalState + std::to_string(num);

			Automaton2_Q.push_back(finalState);

			Automaton2.set_Q(Automaton2_Q);

			//adding finalState to Automaton3's m_F
			std::vector<std::string> Automaton3_F;
			Automaton3_F.push_back(finalState);
			Automaton3.set_F(Automaton3_F);

			//adding all the states to Automaton3's Q
			std::vector<std::string> Automaton3_Q = Automaton3.get_Q();
			Automaton3_Q.push_back(Automaton1.get_q0());

			for (const std::string& state : Automaton1_Q)
			{
				Automaton3_Q.push_back(state);
			}
			for (const std::string& state : Automaton2_Q)
			{
				Automaton3_Q.push_back(state);
			}

			Automaton3.set_Q(Automaton3_Q);

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
			Automaton3_delta.push_back({ Automaton3.get_q0(), emptyString[0], {Automaton1.get_q0(),Automaton2.get_q0()}});
			Automaton3_delta.push_back({ Automaton1.get_F()[0], emptyString[0], {Automaton3.get_F()[0]}});
			Automaton3_delta.push_back({ Automaton2.get_F()[0], emptyString[0], {Automaton3.get_F()[0]}});

			Automaton3.set_delta(Automaton3_delta);

			//adding the alphabet in Automaton3
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

			stateContor += 2;
			AutomatonStack.push(Automaton3);
			std::cout << std::endl;
			Automaton3.PrintAutomaton();
		}
		else if(character == '.')
		{
			NondeterministicFiniteAutomatonWithLambdaTransitions Automaton3;
			NondeterministicFiniteAutomatonWithLambdaTransitions Automaton2 = AutomatonStack.top(); AutomatonStack.pop();
			NondeterministicFiniteAutomatonWithLambdaTransitions Automaton1 = AutomatonStack.top(); AutomatonStack.pop();

			//decreasing the number of every state from Automaton2's m_Q by 1
			std::vector<std::string> Automaton2_Q = Automaton2.get_Q();
			for (std::string& state : Automaton2_Q)
			{
				std::string numPart = state.substr(1);
				state = state[0];

				int num = std::stoi(numPart);
				num--;

				state = state + std::to_string(num);
			}

			//decreasing the number of Automaton2's final state(m_q0)
			std::vector<std::string> Automaton2_F;
			std::string finalState = Automaton2.get_F()[0];
			std::string numPart = finalState.substr(1);
			finalState = finalState[0];

			int num = std::stoi(numPart);
			num -= 1;

			finalState = finalState + std::to_string(num);

			Automaton2_F.push_back(finalState);
			Automaton2.set_F(Automaton2_F);

			//adding Automaton1's m_q0 to Automaton3's m_q0
			Automaton3.set_q0(Automaton1.get_q0());

			//adding Automaton2's m_F to Automaton3's m_F
			Automaton3.set_F(Automaton2.get_F());

			//adding all the states to Automaton3's Q
			std::vector<std::string> Automaton1_Q = Automaton1.get_Q();
			std::vector<std::string> Automaton3_Q ;

			for (const std::string& state : Automaton1_Q)
			{
				Automaton3_Q.push_back(state);
			}
			for (int index=1;index<Automaton2_Q.size();index++)
			{
				Automaton3_Q.push_back(Automaton2_Q[index]);
			}

			Automaton3.set_Q(Automaton3_Q);

			//decreasing the number of every state from Automaton2's m_delta by 1
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
			std::vector<std::tuple<std::string, char, std::vector<std::string>>> Automaton1_delta = Automaton1.get_delta();
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

			//adding the alphabet in Automaton3
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

			stateContor--;
			AutomatonStack.push(Automaton3);
			std::cout << std::endl;
			Automaton3.PrintAutomaton();
		}
		else if(character=='*')
		{
			NondeterministicFiniteAutomatonWithLambdaTransitions Automaton = AutomatonStack.top(); AutomatonStack.pop();

			std::string oldInitialState = Automaton.get_q0();
			std::string oldFinalState = Automaton.get_F()[0];

			//increasing the number of every state from Automaton's m_Q by 1 to add the new starting and ending states
			std::vector<std::string> Automaton_Q = Automaton.get_Q();
			for (std::string& state : Automaton_Q)
			{
				std::string numPart = state.substr(1);
				state = state[0];

				int num = std::stoi(numPart);
				num++;

				state = state + std::to_string(num);
			}

			//increasing the number of Automaton's final state by 2 and adding it to m_Q
			std::vector<std::string> Automaton_F;
			std::string finalState = Automaton.get_F()[0];
			std::string numPart = finalState.substr(1);
			finalState = finalState[0];

			int num = std::stoi(numPart);
			num += 2;

			finalState = finalState + std::to_string(num);

			Automaton_Q.push_back(finalState);
			Automaton_Q.push_back(Automaton.get_q0());
			Automaton.set_Q(Automaton_Q);

			//adding finalState to Automaton3's m_F
			Automaton_F.push_back(finalState);
			Automaton.set_F(Automaton_F);

			//adding the new transitions in Automaton
			std::string emptyString = "";
			std::vector<std::tuple<std::string, char, std::vector<std::string>>> Automaton_delta = Automaton.get_delta();
			Automaton_delta.push_back({ Automaton.get_q0(), emptyString[0], {Automaton.get_F()[0]}});
			Automaton_delta.push_back({ oldFinalState, emptyString[0], {oldInitialState} });

			Automaton.set_delta(Automaton_delta);
			stateContor += 2;
			AutomatonStack.push(Automaton);
			std::cout << std::endl;
			Automaton.PrintAutomaton();
		}
		else
		{
			NondeterministicFiniteAutomatonWithLambdaTransitions Automaton;

			std::vector<std::string> Automaton_Q;

			std::string state = "q";
			state = state + std::to_string(stateContor);
			Automaton_Q.push_back(state);
			Automaton.set_q0(state);

			std::string numPart = state.substr(1);
			state = state[0];

			int num = std::stoi(numPart);
			num++;

			state = state + std::to_string(num);
			Automaton_Q.push_back(state);
			std::vector<std::string> Automaton_F;
			Automaton_F.push_back(state);
			Automaton.set_F(Automaton_F);

			Automaton.set_Q(Automaton_Q);

			std::string Automaton_sigma;
			Automaton_sigma.push_back(character);
			Automaton.set_sigma(Automaton_sigma);

			std::vector<std::tuple<std::string, char, std::vector<std::string>>> Automaton_delta;
			Automaton_delta.push_back({Automaton.get_q0(), character, Automaton.get_F()});
			Automaton.set_delta(Automaton_delta);

			stateContor+=2;
			AutomatonStack.push(Automaton);
			std::cout << std::endl;
			Automaton.PrintAutomaton();
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
