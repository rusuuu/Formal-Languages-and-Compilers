#include <iostream>
#include "RegularExpression.h"
#include "DeterministicFiniteAutomaton.h"

int main()
{
    //RegularExpression expresie("a.b.a.(a.a|b.b)*.c.(a.b)*","");
    RegularExpression expresie("(a.a|b)*.b.b", "");
    //RegularExpression expresie("", "");
    expresie.Output();
    expresie.InfixedToPostfixed();
    expresie.Output();
    NondeterministicFiniteAutomatonWithLambdaTransitions Automaton = expresie.PosfixedToNondeterministicFiniteAutomatonWithLambdaTransitions();

	DeterministicFiniteAutomaton DFA;
    DFA.setAutomaton(Automaton);
    DFA.NondeterministicFiniteAutomatonWithLambdaTransitionsToDeteministicFiniteAutomaton();

}

