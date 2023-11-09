#include <iostream>
#include "Gramatica.h" 

int main() 
{
    
    std::string filePath = "Rules.txt";

   
    Gramatica grammar("S", "ABC", "abc", {}); 

   
    grammar.ReadGrammar(filePath);

   
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

    return 0;
}
