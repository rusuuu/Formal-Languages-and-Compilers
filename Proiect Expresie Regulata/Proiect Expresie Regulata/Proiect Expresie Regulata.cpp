#include <iostream>
#include "RegularExpression.h"

int main()
{
    RegularExpression expresie("a.b.a.(a.a|b.b)*.c.(a.b)*","");
    //RegularExpression expresie("a.b", "");
    //RegularExpression expresie("", "");
    expresie.Output();
    expresie.InfixedToPostfixed();
    expresie.Output();
    NondeterministicFiniteAutomatonWithLambdaTransitions Automaton = expresie.PosfixedToNondeterministicFiniteAutomatonWithLambdaTransitions();
}

