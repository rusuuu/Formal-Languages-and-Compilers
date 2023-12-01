#include <iostream>

#include "Grammar.h"
#include "FiniteAutomaton.h" 

int main() 
{
    
    std::string filePath1 = "Rules.txt";
    std::string filePath2 = "AutomatonInput.txt";
   
    Grammar grammar;
    FiniteAutomaton automaton;

    grammar.ReadGrammar(filePath1);
    automaton.ReadAutomaton(filePath2);
    std::cout << std::endl;
    grammar.PrintRules();
    std::cout << std::endl;
   
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

    std::string word="0110";
    if (automaton.CheckWord(word))
    {
        std::cout << "The word: "<< word <<" is accepted." << std::endl;
    }
    else
    {
        std::cout << "The word: " << word << " isn't accepted." << std::endl;
    }


    //grammar.GenerateWord();

    FiniteAutomaton newAutomaton;
    newAutomaton = newAutomaton.GrammarToFiniteAutomaton(grammar);
    std::cout << std::endl;
    std::cout << std::endl;
    newAutomaton.PrintAutomaton();

    return 0;
}
