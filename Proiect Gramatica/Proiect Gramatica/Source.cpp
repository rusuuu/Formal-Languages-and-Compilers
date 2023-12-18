#include <iostream>
#include <unordered_set>

#include "Grammar.h"
#include "FiniteAutomaton.h" 

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
	std::string filePath1 = "Rules.txt";
	std::string filePath2 = "AutomatonInput.txt";

	Grammar grammar;
	FiniteAutomaton automaton;

	grammar.ReadGrammar(filePath1);
	automaton = automaton.GrammarToFiniteAutomaton(grammar);

	if (grammar.IsValidGrammar())
	{
		std::cout << GREEN "The grammar is valid." << std::endl;
	}
	else
	{
		std::cout << RED "The grammar is not valid." << std::endl;
		return 1;
	}

	if (grammar.IsRegular())
	{
		std::cout << GREEN "The grammar is regular." << std::endl;
	}
	else
	{
		std::cout << RED "The grammar is not regular." << std::endl;
		return 1;
	}

	int choice;
	do
	{
		displayHeader();
		std::cout << "1. Display grammar\n";
		std::cout << "2. Generate n words in grammar\n";
		std::cout << "3. Display equivalent automaton\n";
		std::cout << "4. Check if a word is accepted by the automaton\n";
		std::cout << "5. Generate a word in G and check if it is accepted by the automaton\n";
		std::cout << "6. Clear screen.\n";
		std::cout << "7. Quit\n";

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
				std::unordered_set<std::string> checkUniqueness;

				int number;
				std::cout << "Enter the number of words to generate: ";
				std::cin >> number;


				if (std::cin.fail())
				{
					std::cin.clear();
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					std::cout << RED "You entered a character. Please enter a number!\n";
				}
				else
				{
					while (number <= 0)
					{
						std::cout << RED "Invalid number of words. Try again!\n" << RESET;
						std::cout << "Enter the number of words to generate: ";
						std::cin >> number;
					}

					std::cout << "\nGenerated words:\n\n";
					for (int index = 0; index < number; index++)
					{
						word = grammar.GenerateWord();
						auto it = checkUniqueness.find(word);

						if (it == checkUniqueness.end())
						{
							checkUniqueness.insert(word);
							std::cout << "Generated word " << index + 1 << ": " << word << std::endl << std::endl;
						}
						else
							index--;
					}
				}
				break;
			}
			case 3:
			{
				std::cout << "Equivalent automaton:\n\n";
				automaton.PrintAutomaton();
				break;
			}
			case 4:
			{
				if (!automaton.VerifyAutomaton())
				{
					std::cout << RED "Automaton not valid!\n";
				}
				else
				{
					std::string word;

					std::cout << "Enter a word to check: ";
					std::cin >> word;
					bool accepted = automaton.CheckWord(word);
					std::cout << "The word is " << (accepted ? GREEN "accepted" : RED "rejected") << RESET << " by the automaton.\n";
				}
				break;
			}
			case 5:
			{
				std::string word;

				std::cout << "Generated word in G:\n";
				word = grammar.GenerateWord();
				bool accepted = automaton.CheckWord(word);
				std::cout << "The word is " << (accepted ? GREEN "accepted" : RED "rejected") << RESET << " by the automaton.\n";
				break;
			}
			case 6:
			{
				system("clear || cls");

				break;
			}
			case 7:
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
	} while (choice>=0 && choice <=7);
}

