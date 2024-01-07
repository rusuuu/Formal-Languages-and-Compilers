#include <iostream>
#include <unordered_set>

#include "Grammar.h"
#include "PushDownAutomaton.h" 

#define RESET   "\033[0m"
#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define PURPLE  "\033[35m"
#define GREY	"\033[90m"

void displayHeader() {
	std::cout << "\033[1;33m\n==============================\n";
	std::cout << "        PROGRAM MENIU          \n";
	std::cout << "==============================\n\n";
}

int main()
{
	std::string filePath = "grammar.txt";

	Grammar grammar;
	PushDownAutomaton automaton;
	std::unordered_set<std::string> checkUniqueness;

	grammar.ReadGrammar(filePath);
	automaton = automaton.IDCtoPDAconversion(grammar);

	if (grammar.IsValidGrammar())
	{
		std::cout << GREEN "The grammar is valid." << std::endl;
	}
	else
	{
		std::cout << RED "The grammar is not valid." << std::endl;
		return 1;
	}

	if (grammar.IsContextFree())
	{
		std::cout << GREEN "The grammar is context free." << std::endl;
	}
	else
	{
		std::cout << RED "The grammar is not context free." << std::endl;
		return 1;
	}

	int choice;
	do
	{
		displayHeader();
		std::cout << "1. Display grammar\n";
		std::cout << "2. Generate a word in grammar\n";
		std::cout << "3. Display simplified grammar\n";
		std::cout << "4. Display grammar in GNF form\n";
		std::cout << "5. Display push down automaton\n";
		std::cout << "6. Generate a word in G and check if it is accepted by the automaton\n";
		std::cout << "7. Verify if a word is accepted by the automaton\n";
		std::cout << "8. Clear screen.\n";
		std::cout << "9. Quit\n";

		std::cout << PURPLE "Enter your choice: ";
		std::cin >> choice;

		std::cout << std::endl << RESET;

		if (std::cin.fail())
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << RED "You entered a character. Please enter a number!\n";
		}
		else
		{
			switch (choice)
			{
			case 1:
			{
				std::cout << "Grammar:\n\n";
				grammar.PrintGrammar();
				break;
			}
			case 2:
			{
				std::string word;
				

				std::cout << "\nGenerated word:\n\n";
				std::string generatedWord = grammar.GenerateWord();

				auto it = checkUniqueness.find(generatedWord);

				while (it != checkUniqueness.end())
				{
					generatedWord = grammar.GenerateWord();
					it = checkUniqueness.find(generatedWord);
				}

				checkUniqueness.insert(generatedWord);
				std::cout << "Generated word: " << generatedWord << std::endl << std::endl;
				
				break;
			}
			case 3:
			{
				std::cout << "Simplified Grammar:\n\n";
				grammar.SimplifyGrammar();
				grammar.PrintGrammar();
				break;
			}
			case 4:
			{
				std::cout << "Grammar in GNF form:\n\n";
				grammar.ConvertToGNFwithPrint();
				break;
			}
			case 5:
			{
				std::cout << "Push Down Automaton: \n";
				automaton.PrintAutomaton();
				break;
			}
			case 6:
			{
				std::string word;
				word = grammar.GenerateWord();
				std::cout << "Generated word in G: "<<word<<"\n";
				
				bool accepted = automaton.CheckWord(word);
				std::cout << "The word is " << (accepted ? GREEN "accepted" : RED "rejected") << RESET << " by the automaton.\n";
				break;
			}
			case 7:
			{
				std::cout << "Insert a word to check: ";
				std::string word;
				std::cin >> word;
				bool accepted = automaton.CheckWord(word);
				std::cout << "The word is " << (accepted ? GREEN "accepted" : RED "rejected") << RESET << " by the automaton.\n";
				break;
			}
			case 8:
			{
				system("clear || cls");

				break;
			}
			case 9:
			{
				std::cout << GREEN "Exiting the program. Goodbye!\n" << RESET;
				return 0;
			}
			default:
			{
				std::cout << RED "Invalid choice. Please try again!\n" << RESET;
				std::cout << GREY << "*Enter a number between 1 and 6.\n" << RESET;
			}
			}
		}
	} while (choice >= 0 && choice <= 8);
}

