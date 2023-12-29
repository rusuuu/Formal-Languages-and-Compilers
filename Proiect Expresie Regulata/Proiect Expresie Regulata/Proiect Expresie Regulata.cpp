#include <iostream>
#include "RegularExpression.h"

int main()
{
    RegularExpression expresie("a.b.a.(a.a|b.b)*.c.(a.b)*","");
    expresie.Output();
    expresie.InfixedToPostfixed();
    expresie.Output();
}

