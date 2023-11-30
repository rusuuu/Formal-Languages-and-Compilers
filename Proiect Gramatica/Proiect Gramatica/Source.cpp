#include <iostream>

#include "Gramatica.h"
#include "AutomatFinit.h" 

int main() 
{
    
    std::string filePath1 = "Rules.txt";
    std::string filePath2 = "AutomatonInput.txt";
   
    Gramatica grammar;
    AutomatFinit automaton;

    grammar.ReadGrammar(filePath1);
    automaton.ReadAutomaton(filePath2);
   
    if (grammar.IsValidGrammar()) 
    {
        std::cout << "The grammar is valid." << std::endl;
    }
    else 
    {
        std::cout << "The grammar is not valid." << std::endl;
        return 1;  
    }

   
    if (grammar.IsRegular()) 
    {
        std::cout << "The grammar is regular." << std::endl;
    }
    else 
    {
        std::cout << "The grammar is not regular." << std::endl;
    }

    if (automaton.VerifyAutomaton())
    {
        std::cout << "The automaton is valid." << std::endl;
    }
    else
    {
        std::cout << "The automaton is not valid." << std::endl;
        return 1;
    }

    if (automaton.IsDeterministic())
    {
        std::cout << "The automaton is determinisitc." << std::endl;
    }
    else
    {
        std::cout << "The automaton is non-deterministic." << std::endl;
    }

    //automaton.PrintAutomaton();

    std::string word="aab";
    if (automaton.CheckWord(word))
    {
        std::cout << "The word is accepted." << std::endl;
    }
    else
    {
        std::cout << "The word isn't accepted." << std::endl;
    }

    return 0;
}
