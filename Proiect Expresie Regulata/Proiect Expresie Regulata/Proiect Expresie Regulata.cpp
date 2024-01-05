#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "RegularExpression.h"
#include "DeterministicFiniteAutomaton.h"

void DisplayMenu()
{
    std::cout << "\nMenu:\n"
        << "  (a) Display the DFA in console and output to a file.\n"
        << "  (b) Display the regular expression from the file.\n"
        << "  (c) Check a word against the DFA.\n"
        << "  (d) Clear screen.\n"
        << "  (q) Quit.\n"
        << "Enter your choice: ";
}

bool CheckWord(DeterministicFiniteAutomaton& dfa, const std::string& word) 
{
    return dfa.CheckWord(word);
}

void ClearScreen() {
    // Clear the screen based on the operating system
#if defined _WIN32
    system("cls");
#else
    // Assume POSIX
    system("clear");
#endif
}

int main() 
{
    std::string line;
    std::ifstream file("regular_expression.txt");

    if (file.is_open()) 
    {
        if (std::getline(file, line))
        {
            RegularExpression expression(line, "");
            if (!expression.VerifyExpression())
            {
                throw std::exception("Invalid regular expression!");
                
            }
            
            expression.InfixedToPostfixed();
            auto nfa = expression.PosfixedToNondeterministicFiniteAutomatonWithLambdaTransitions();

            DeterministicFiniteAutomaton dfa;
            dfa.setAutomaton(nfa);
            DeterministicFiniteAutomaton dfaPrint = dfa.NondeterministicFiniteAutomatonWithLambdaTransitionsToDeteministicFiniteAutomaton();

            char choice;
            do {
                DisplayMenu();
                std::cin >> choice;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                switch (choice) 
                {
                case 'a': 
                {                    
                    dfaPrint.PrintAutomaton();
                    break;
                }
                case 'b': 
                {
                    expression.Output();
                    break;
                }
                case 'c': 
                {
                    std::string word;
                    std::cout << "Enter a word to check: ";
                    std::cin >> word;
                    if (CheckWord(dfaPrint, word)) 
                    {
                        std::cout << "The word is accepted by the DFA." << std::endl;
                    }
                    else 
                    {
                        std::cout << "The word is not accepted by the DFA." << std::endl;
                    }
                    break;
                }
                case 'd':
                {
                    ClearScreen();
                    break;
                }
                if (choice != 'q') 
                {
                    std::cout << "\nPress Enter to continue...";
                    std::cin.get();
                    ClearScreen();
                }
                }
            } while (choice != 'q');
        }
        file.close();
    }
    else 
    {
        std::cerr << "Unable to open file";
    }

    return 0;
}
