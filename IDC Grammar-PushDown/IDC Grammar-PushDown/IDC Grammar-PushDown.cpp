#include <iostream>
#include "Grammar.h"
#include "PushDownAutomaton.h"

int main() {
    // Read the grammar from the file
    Grammar g;
    g.ReadGrammar("grammar.txt");
    std::cout << "Original Grammar:\n";
    g.PrintGrammar();

    /*if (g.IsContextFree())
    {
        std::cout << "it is context free" << std::endl;
    }
    else {
        std::cout << "it is NOT context free" << std::endl;

    }*/

    //Simplify the grammar
   /* g.RemoveNullProductions();
    std::cout << "\nAfter Removing Null Productions:\n";
    g.PrintGrammar();

    g.RemoveUnitProductions();
    std::cout << "\nAfter Removing Unit Productions:\n";
    g.PrintGrammar();

    g.RemoveUnproductiveSymbols();
    std::cout << "\nAfter Removing Unproductive Symbols:\n";
    g.PrintGrammar();

    g.RemoveUnreachableSymbols();
    std::cout << "\nAfter Removing Unreachable Symbols:\n";
    g.PrintGrammar();*/


    g.ConvertToGNF();
    std::cout << "\nAfter GNF:\n";
    g.PrintGrammar();

    /*g.RemoveStartSymbolFromRHS();
    std::cout << "\nRemoveStartSymbolFromRHS:\n";
    g.PrintGrammar();*/
    
    /*g.RemoveNullProductions();
    std::cout << "\nRemove NUll Productions:\n";
    g.PrintGrammar();*/

    /*g.RemoveUnitProductions();
    std::cout << "\nRemove Unit Productions:\n";
    g.PrintGrammar();

    g.RemoveUnreachableSymbols();
    std::cout << "\nRemove Unreacheable Productions:\n";
    g.PrintGrammar();

    g.ConvertToCNF();
    std::cout << "\nAfter CNF:\n";
    g.PrintGrammar();

    g.EliminateImmediateLeftRecursion();
    std::cout << "\nEliminate Immediate Left Recursion:\n";
    g.PrintGrammar();
    
    g.ConvertRulesToGNF(); 
    std::cout << "\nAfter GNF:\n";
    g.PrintGrammar(); */
    

    //-----------------------------PushDownAutomaton---------------------------------------------

    /*std::vector<std::string> Q = { "q0", "q1", "q2" };
    std::string sigma = "ab";
    std::string gamma = "A";
    std::string q0 = "q0";
    std::string Z0 = "Z0";
    std::vector<std::string> F = { "q2" };

    std::vector<std::tuple<std::string, char, std::string, std::vector<std::pair<std::string, std::vector<std::string>>>>> delta = {
        std::make_tuple("q0", 'a', "Z0", std::vector<std::pair<std::string, std::vector<std::string>>>{{"q0", {"A", "Z0"}}}),
        std::make_tuple("q0", 'a', "A", std::vector<std::pair<std::string, std::vector<std::string>>>{{"q0", {"A", "A"}}}),
        std::make_tuple("q0", 'b', "A", std::vector<std::pair<std::string, std::vector<std::string>>>{{"q1", {}}}),
        std::make_tuple("q1", 'b', "A", std::vector<std::pair<std::string, std::vector<std::string>>>{{"q1", {}}}),
        std::make_tuple("q1", '\0', "Z0", std::vector<std::pair<std::string, std::vector<std::string>>>{{"q2", {"Z0"}}})
    };

    PushDownAutomaton pda(Q, sigma, gamma, q0, Z0, F, delta);
    pda.PrintAutomaton();
    bool check = pda.CheckWord("aaabbb");

    if (check == true) std::cout << "TRUE";
    else
        std::cout << "FALSE";*/

    return 0;
}

/*
S -> AB
S ->
A -> a
A -> CA
B -> b
B -> SB
C -> c
D -> d
D ->
E -> EE
E -> e
F -> fF
F -> */