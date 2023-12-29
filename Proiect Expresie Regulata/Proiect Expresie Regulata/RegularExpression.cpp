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

void RegularExpression::Output()
{
	std::cout << "Infixed Form: " << m_Infixed << std::endl;
	std::cout << "Postfixed Form: " << m_Postfixed << std::endl;
	std::cout << std::endl;
}
