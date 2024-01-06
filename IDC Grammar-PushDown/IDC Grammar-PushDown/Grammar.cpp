#include "Grammar.h"

#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <ctime>
#include <set>
#include <queue>

Grammar::Grammar(std::string startSymbol, std::string Vn, std::string Vt, std::multimap<std::string, std::string> Rules, int zCounter) :
    m_startSymbol(std::move(startSymbol)),
    m_VN(std::move(Vn)),
    m_VT(std::move(Vt)),
    m_PRules(std::move(Rules)),
    m_zCounter(1) 
{
}

Grammar::Grammar()
{
}

bool Grammar::IsValidGrammar()
{
    
    if (m_VN.empty() || m_VT.empty())
    {
        std::cerr << "VN or VT is empty\n"; 
        return false;
    }

    
    if (m_VN.find_first_of(m_startSymbol) == std::string::npos)
    {
        std::cerr << "Start symbol not in VN\n"; 
        return false;
    }

    
    if (m_PRules.empty())
    {
        std::cerr << "Production rules are empty\n"; 
        return false;
    }

    
    if (m_VN.find(m_VT) != std::string::npos)
    {
        std::cerr << "Overlap between VN and VT\n"; 
        return false;
    }

    return true;
}

void Grammar::ReadGrammar(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Unable to open file\n";
        return;
    }

    std::string line;
    
    if (std::getline(file, line))
    {
        m_startSymbol = Trim(line);
    }

    
    if (std::getline(file, line))
    {
        m_VN = Trim(line);
    }

    
    if (std::getline(file, line))
    {
        m_VT = Trim(line);

    }

    while (std::getline(file, line))
    {
        line = Trim(line);
        size_t pos = line.find("->");
        if (pos != std::string::npos)
        {
            std::string left = Trim(line.substr(0, pos));
            std::string right = Trim(line.substr(pos + 2));

            m_PRules.insert(std::make_pair(left, right));
        }
    }

    file.close();
}

void Grammar::DisplayGrammar(const std::string& filename)
{

    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Unable to open file\n";
        return;
    }

    std::string line;

    
    if (std::getline(file, line))
    {
        m_startSymbol = Trim(line);
        std::cout << "Start symbol: " << m_startSymbol << "\n";
    }

    
    if (std::getline(file, line))
    {
        m_VN = Trim(line);
        std::cout << "Non-terminals: " << m_VN << "\n";
    }

    
    if (std::getline(file, line))
    {
        m_VT = Trim(line);
        std::cout << "Terminals: " << m_VT << "\n";
    }

    
    std::cout << "Production rules:\n";
    while (std::getline(file, line))
    {
        line = Trim(line);
        size_t pos = line.find("->");
        if (pos != std::string::npos)
        {
            std::string left = Trim(line.substr(0, pos));
            std::string right = Trim(line.substr(pos + 2));

            m_PRules.insert(std::make_pair(left, right));
            std::cout << "New print: " << left << " -> " << right << "\n";
        }
    }

    file.close();
}

void Grammar::PrintGrammar() 
{
    // Display non-terminals
    std::cout << "Non-terminals: " << m_VN << "\n";

    // Display terminals
    std::cout << "Terminals: " << m_VT << "\n";

    // Display production rules
    std::cout << "Production rules:\n";
    for (const auto& rule : m_PRules) {
        std::cout << rule.first << " -> " << rule.second << "\n";
    }
}



bool Grammar::IsRegular()
{
    bool isRightLinear = true;
    bool isLeftLinear = true;

    for (const auto& rule : m_PRules)
    {
        const std::string& left = rule.first;
        const std::string& right = rule.second;

        if (left.length() != 1 || !IsNonTerminal(left[0]))
        {
            return false;
        }

        if (!right.empty())
        {
            if (IsNonTerminal(right.back()) && right.length() == 1)
            {
                continue;
            }
            else if (IsNonTerminal(right.back()))
            {
                isLeftLinear = false;
            }
            else if (IsNonTerminal(right.front()))
            {
                isRightLinear = false;
            }
            else if (!IsTerminalString(right))
            {
                return false;
            }
        }
    }

    return isRightLinear || isLeftLinear;
}

bool Grammar::IsNonTerminal(const char character)
{
    return m_VN.find(character) != std::string::npos;
}

std::string Grammar::GetVN()
{
    return m_VN;
}

std::string Grammar::GetVT()
{
    return m_VT;
}

std::string Grammar::GetStartSymbol()
{
    return m_startSymbol;
}

std::multimap<std::string, std::string> Grammar::GetPRules()
{
    return m_PRules;
}


bool Grammar::IsTerminalString(const std::string& inputString)
{
    for (char character : inputString)
    {
        if (m_VT.find(character) == std::string::npos)
        {
            return false;
        }
    }
    return true;
}

std::mt19937 Grammar::rng(static_cast<unsigned int>(std::time(nullptr))); 

std::string Grammar::GenerateWord()
{
    if (!IsValidGrammar())
    {
        std::cerr << "Invalid grammar, cannot generate word\n";
        return "";
    }

    std::string currentString = m_startSymbol;
    std::string progress = m_startSymbol; 

    int maxIterations = 100;
    int iterationCount = 0;

    while (iterationCount < maxIterations && std::any_of(currentString.begin(), currentString.end(), [this](char ch) { return IsNonTerminal(ch); }))
    {
        std::vector<std::pair<std::string, std::string>> applicableProductions;

        
        for (const auto& rule : m_PRules)
        {
            size_t pos = currentString.find(rule.first);
            if (pos != std::string::npos)
            {
                applicableProductions.emplace_back(rule.first, rule.second);
            }
        }

        if (applicableProductions.empty())
        {
            break; 
        }

        
        std::uniform_int_distribution<int> dist(0, applicableProductions.size() - 1);
        auto selectedProduction = applicableProductions[dist(rng)];

        
        size_t pos = currentString.find(selectedProduction.first);
        if (pos != std::string::npos) 
        {
            currentString.replace(pos, selectedProduction.first.length(), selectedProduction.second);
        }

        
        progress += " -> " + currentString;

        iterationCount++;
    }

    if (iterationCount >= maxIterations) 
    {
        std::cerr << "Reached maximum iterations, might be stuck in a loop\n";
    }

    std::cout << "The process: " << progress << std::endl;
    return currentString;
}

//--------------------------------IDC Grammar------------------------------------------------------------------------------------------

bool Grammar::IsContextFree()
{
    if (m_PRules.empty()) 
    {
        return false; // A valid CFG must have at least one production rule
    }

    for (const auto& rule : m_PRules)
    {
        // Check for a single non-terminal on the left-hand side
        if (rule.first.length() != 1 || !IsNonTerminal(rule.first[0])) 
        {
            return false;
        }

        // Check the right-hand side of the rule
        if (!IsRightHandSideValid(rule.second))
        {
            return false;
        }
    }
    return true;
}


bool Grammar::IsRightHandSideValid(const std::string& rhs)
{
    // Check if the right-hand side is valid for a context-free grammar
    if (rhs.empty()) 
    {
        return false;  // In CFG, the RHS can't be empty (that's for regular grammars)
    }

    for (const char& symbol : rhs) 
    {
        // Check if each symbol is either a valid terminal or non-terminal
        if (m_VN.find(symbol) == std::string::npos && m_VT.find(symbol) == std::string::npos)
        {
            return false;  // Invalid symbol found
        }
    }
    // Additional checks can be implemented as needed
    return true;
}


//--------------------------------Simplify Grammar--------------------------------------------------------------------------------------

void Grammar::SimplifyGrammar() 
{
    RemoveNullProductions();  // 1. Elimination of null-productions
    RemoveUnitProductions();  // 2. Elimination of unit productions (renamings)
    RemoveUnproductiveSymbols();   // 3. Elimination of symbols that don't derive terminal strings
    RemoveUnreachableSymbols(); // 4. Elimination of unreachable symbols
}

void Grammar::RemoveUnproductiveSymbols()
{
    std::set<std::string> productiveSymbols;

    // Step 1: Initially, add all symbols that directly lead to terminal strings as productive.
    for (const auto& rule : m_PRules)
    {
        if (IsTerminalString(rule.second)) 
        {
            // Check if the right-hand side is a terminal string
            productiveSymbols.insert(rule.first);
        }
    }

    // Step 2: Iteratively identify indirectly productive symbols.
    bool changed = true;
    while (changed) 
    {
        changed = false;
        for (const auto& rule : m_PRules) 
        {
            if (productiveSymbols.count(rule.first) == 0) 
            {
                // Check if all symbols in the right-hand side are terminals or productive non-terminals.
                bool allProductive = true;
                for (const char& c : rule.second) 
                {
                    if (m_VT.find(c) == std::string::npos && productiveSymbols.count(std::string(1, c)) == 0) 
                    {
                        allProductive = false;
                        break;
                    }
                }
                if (allProductive)
                {
                    productiveSymbols.insert(rule.first);
                    changed = true;
                }
            }
        }
    }

    // Step 3: Remove unproductive rules and update non-terminals.
    for (auto it = m_PRules.begin(); it != m_PRules.end();) 
    {
        if (productiveSymbols.count(it->first) == 0) 
        {
            it = m_PRules.erase(it);
        }
        else 
        {
            ++it;
        }
    }

    // Update the list of non-terminals to only include productive ones.
    std::string newVN;
    for (const char& nt : m_VN)
    {
        if (productiveSymbols.count(std::string(1, nt)))
        {
            newVN += nt;
        }
    }
    m_VN = newVN;
}


void Grammar::RemoveUnreachableSymbols() 
{
    std::set<std::string> reachableSymbols;  // To keep track of all reachable symbols (both terminals and non-terminals)
    std::queue<std::string> symbolsToExplore;  // Queue to manage exploration

    // Start symbol is always reachable
    reachableSymbols.insert(m_startSymbol);
    symbolsToExplore.push(m_startSymbol);

    while (!symbolsToExplore.empty())
    {
        std::string currentSymbol = symbolsToExplore.front();
        symbolsToExplore.pop();

        // Explore all productions where the current symbol is on the left-hand side
        for (const auto& rule : m_PRules) 
        {
            if (rule.first == currentSymbol) 
            {
                // Check each character in the production's right-hand side
               
                for (const char& c : rule.second) 
                {
                    
                    std::string strSymbol(1, c);  // Convert char to string for comparison and insertion

                    // If it's a non-terminal and not already marked as reachable
                    if (IsNonTerminal(c) && reachableSymbols.find(strSymbol) == reachableSymbols.end()) 
                    {
                        reachableSymbols.insert(strSymbol);  // Mark it as reachable
                        symbolsToExplore.push(strSymbol);  // Add it to the queue for further exploration
                    }
                    else if (!IsNonTerminal(c)) 
                    {
                        reachableSymbols.insert(strSymbol);  // Directly mark terminals as reachable
                    }
                }
            }
        }
    }

    // Now, remove all unreachable non-terminals and their productions
    for (auto it = m_PRules.begin(); it != m_PRules.end();) 
    {
        if (reachableSymbols.find(it->first) == reachableSymbols.end())
        {  // If the left-hand side is unreachable
            it = m_PRules.erase(it);  // Remove the production
        }
        else 
        {
            ++it;
        }
    }

    // Update VN to remove unreachable non-terminals
    std::string newVN;
    for (const char& nt : m_VN) 
    {
        if (reachableSymbols.find(std::string(1, nt)) != reachableSymbols.end())
        { 
            // If the non-terminal is reachable
            newVN += nt;  // Keep it
        }
    }
    m_VN = newVN;

    // Update VT to remove unreachable terminals
    std::string newVT;
    for (const char& t : m_VT) 
    {
        if (reachableSymbols.find(std::string(1, t)) != reachableSymbols.end()) 
        {
            // If the terminal is reachable
            newVT += t;  // Keep it
        }
    }
    m_VT = newVT;
}



// Function to remove unit productions (renamings)
void Grammar::RemoveUnitProductions()
{
    bool changes = true;

    while (changes)
    {
        changes = false;
        std::multimap<std::string, std::string> newPRules = m_PRules;

        // Identify all unit productions.
        std::vector<std::pair<std::string, std::string>> unitProductions;
        for (const auto& rule : m_PRules)
        {
            if (rule.second.length() == 1 && m_VN.find(rule.second) != std::string::npos)
            {
                unitProductions.emplace_back(rule.first, rule.second);
            }
        }

        // Replace each unit production with the productions of the non-terminal on the RHS.
        for (const auto& unitProd : unitProductions)
        {
            std::string from = unitProd.first;
            std::string to = unitProd.second;

            auto range = m_PRules.equal_range(to);
            for (auto it = range.first; it != range.second; ++it)
            {
                // Skip adding a production if it would create a new unit production.
                if (it->second != from && it->second.length() != 1)
                {
                    newPRules.insert({ from, it->second });
                    changes = true;
                }
            }
        }

        // Remove the original unit productions.
        for (const auto& unitProd : unitProductions)
        {
            auto range = newPRules.equal_range(unitProd.first);
            for (auto it = range.first; it != range.second;)
            {
                if (it->second == unitProd.second)
                {
                    it = newPRules.erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }

        m_PRules = newPRules;
    }
}





std::set<std::string> Grammar::FindNullableNonTerminals() 
{
    std::set<std::string> nullableNonTerminals;
    bool changed;

    do
    {
        changed = false;
        for (const auto& rule : m_PRules)
        {
            if ((rule.second == "?" || rule.second.empty()) && nullableNonTerminals.insert(rule.first).second) 
            {
                changed = true;
            }
            else
            {
                bool allNullable = true;
                for (char c : rule.second)
                {
                    if (nullableNonTerminals.count(std::string(1, c)) == 0) 
                    {
                        allNullable = false;
                        break;
                    }
                }
                if (allNullable) 
                {
                    if (nullableNonTerminals.insert(rule.first).second) 
                    {
                        changed = true;
                    }
                }
            }
        }
    } while (changed);

    return nullableNonTerminals;
}

std::set<std::string> Grammar::GenerateCombinations(const std::string& production, const std::set<std::string>& nullableNonTerminals) 
{
    std::set<std::string> combinations;

    // Base case: If production is empty or all characters are not nullable, return.
    if (production.empty()) 
    {
        return combinations;
    }
    combinations.insert(production);

    for (size_t i = 0; i < production.size(); ++i) 
    {
        if (nullableNonTerminals.find(std::string(1, production[i])) != nullableNonTerminals.end()) 
        {
            std::string reduced = production;
            reduced.erase(i, 1);
            std::set<std::string> subcombinations = GenerateCombinations(reduced, nullableNonTerminals);
            combinations.insert(subcombinations.begin(), subcombinations.end());
        }
    }

    return combinations;
}

void Grammar::RemoveNullProductions()
{
    std::set<std::string> nullableNonTerminals = FindNullableNonTerminals();
    std::multimap<std::string, std::string> newRules;

    for (const auto& rule : m_PRules)
    {
        if (rule.second != "?") 
        {
            std::set<std::string> newProductions = GenerateCombinations(rule.second, nullableNonTerminals);
            for (const std::string& prod : newProductions) 
            {
                if (!prod.empty()) 
                {
                    newRules.insert({ rule.first, prod });
                }
            }
        }
    }

    if (nullableNonTerminals.find(m_startSymbol) != nullableNonTerminals.end()) 
    {
        std::string newStartSymbol = "S0"; // Assuming S0 is not already used.
        newRules.insert({ newStartSymbol, "" });
        newRules.insert({ newStartSymbol, m_startSymbol });
        m_startSymbol = newStartSymbol;
        m_VN.append(newStartSymbol);
    }

    m_PRules = newRules;
}

//------------------------------FNG-------------------------------------------------------------------------------------------

void Grammar::ConvertToGNFwithPrint()
{
    std::cout << "\nRemove start from rhs\n";
    RemoveStartSymbolFromRHS();
    PrintGrammar();

    std::cout << "\nRemove null\n";
    RemoveNullProductions();
    PrintGrammar();

    std::cout << "\nRemove unit\n";
    RemoveUnitProductions();
    PrintGrammar();

    std::cout << "\nRemove unreacheable\n";
    RemoveUnreachableSymbols();
    PrintGrammar();

    std::cout << "\nCNF\n";
    ConvertToCNF();
    PrintGrammar();

    std::cout << "\nRemove immediate left recursion\n";
    EliminateImmediateLeftRecursion();
    PrintGrammar();

    std::cout << "\nGNF\n";
    ConvertRulesToGNF();
    PrintGrammar();

}

void Grammar::ConvertToGNF()
{
    RemoveStartSymbolFromRHS();
    
    RemoveNullProductions();

    RemoveUnitProductions();

    RemoveUnreachableSymbols();

    ConvertToCNF();

    EliminateImmediateLeftRecursion();

    ConvertRulesToGNF();

}

bool Grammar::IsStartSymbolOnRHS() 
{
    for (const auto& rule : m_PRules) 
    {
        if (rule.first != m_startSymbol) 
        {
            // Skip rules where the LHS is the start symbol itself
            for (const auto& prod : rule.second) 
            {
                if (std::string(1, prod).find(m_startSymbol) != std::string::npos) 
                {
                    return true;
                }
            }
        }
    }
    return false;
}

void Grammar::RemoveStartSymbolFromRHS() 
{
    if (IsStartSymbolOnRHS())
    {
        // Create a new start symbol
        std::string newStartSymbol = m_startSymbol + "'";
        while (m_PRules.find(newStartSymbol) != m_PRules.end()) 
        { // Ensure the new start symbol is unique
            newStartSymbol += "'";
        }

        // Add a new production for the new start symbol
        m_PRules.insert({ newStartSymbol, m_startSymbol });

        // Update the start symbol
        m_startSymbol = newStartSymbol;
    }
}


void Grammar::EliminateImmediateLeftRecursion()
{
    std::multimap<std::string, std::string> newPRules;

    for (const auto& ntChar : m_VN)
    {
        std::string nt(1, ntChar);

        std::vector<std::string> alpha;
        std::vector<std::string> beta;
        auto range = m_PRules.equal_range(nt);

        for (auto it = range.first; it != range.second; ++it)
        {
            if (it->second[0] == nt[0]) 
            { // Immediate left recursion A -> Aα
                alpha.push_back(it->second.substr(1)); // Collect α
            }
            else 
            {
                beta.push_back(it->second); // Collect β
            }
        }

        if (!alpha.empty()) 
        {
            // If there's immediate left recursion
            std::string newNonTerminal = nt + "'";
            m_VN += newNonTerminal; // Add the new non-terminal

            // Add A' -> αA' | ε rules
            for (const auto& a : alpha) 
            {
                newPRules.insert({ newNonTerminal, a + newNonTerminal });
            }
            newPRules.insert({ newNonTerminal, "" }); // ε-production

            // Replace original A -> Aα rules with A -> βA'
            for (const auto& b : beta) 
            {
                newPRules.insert({ nt, b + newNonTerminal });
            }
        }
        else 
        { // No immediate left recursion, keep the original rules
            for (auto it = range.first; it != range.second; ++it) 
            {
                newPRules.insert({ it->first, it->second });
            }
        }
    }

    m_PRules = newPRules; // Update the production rules
}

bool Grammar::IsTerminal(const std::string& symbol) 
{
    return symbol.length() == 1 && m_VT.find(symbol[0]) != std::string::npos;
}

// Helper function to apply Lemma 1
void Grammar::ApplyLemma1(std::string lhs, std::string rhs, std::multimap<std::string, std::string>& newPRules) {
    char nonTerminal = rhs[0];
    std::string rest = rhs.substr(1);

    auto replacements = m_PRules.equal_range(std::string(1, nonTerminal));
    for (auto it = replacements.first; it != replacements.second; ++it) {
        newPRules.insert({ lhs, it->second + rest });
    }
}

// Helper function to apply Lemma 2
void Grammar::ApplyLemma2(std::string lhs, std::string rhs, std::multimap<std::string, std::string>& newPRules) 
{
    std::string alpha = rhs.substr(1); // Remove the leading non-terminal (recursive part)
    std::string newNonTerminal = "Z" + std::to_string(m_zCounter++); // Create a new non-terminal

    // Add the new rules replacing the left recursion
    newPRules.insert({ lhs, rhs[0] + newNonTerminal });
    newPRules.insert({ newNonTerminal, alpha });
    newPRules.insert({ newNonTerminal, alpha + newNonTerminal });
}

void Grammar::ConvertRulesToGNF() 
{
    std::multimap<std::string, std::string> newPRules;

    for (auto& rule : m_PRules) {
        std::string lhs = rule.first;
        std::string rhs = rule.second;

        if (IsTerminal(rhs)) {
            newPRules.insert({ lhs, rhs }); // Rule is already in GNF (A -> a)
        }
        else if (rhs.length() >= 2) {
            if (IsTerminal(std::string(1, rhs[0]))) {
                newPRules.insert({ lhs, rhs }); // Already in GNF
            }
            else {
                if (rhs[0] == lhs[0]) {
                    // Lemma 2: Eliminate left recursion
                    ApplyLemma2(lhs, rhs, newPRules);
                }
                else {
                    // Lemma 1: Replace the non-terminal
                    ApplyLemma1(lhs, rhs, newPRules);
                }
            }
        }
    }

    m_PRules = newPRules; // Update the production rules
}


//---------------------CNF----------------------------------------------------------

void Grammar::ConvertToCNF() 
{
    // Introduce Non-terminals for Terminals in RHS
    for (auto& rule : m_PRules) 
    {
        std::string lhs = rule.first;
        std::string rhs = rule.second;

        if (rhs.length() == 1 && m_VT.find(rhs[0]) != std::string::npos) 
        {
            continue; // Skip as it's already in CNF (A -> a)
        }
        else if (rhs.length() >= 2) 
        {
            ReplaceTerminals(rhs);
        }
    }

    // Break Down Longer Productions
    std::multimap<std::string, std::string> newProductions;
    for (auto& rule : m_PRules)
    {
        BreakDownLongerProductions(rule.first, rule.second, newProductions);
    }

    m_PRules = newProductions; // Replace old productions with the new CNF productions
}

void Grammar::ReplaceTerminals(std::string& rhs)
{
    std::map<char, std::string> terminalToNonTerminal; // Map to track which terminals have been replaced
    std::string newRHS;

    for (char& symbol : rhs)
    {
        if (IsNonTerminal(symbol)) // Keep non-terminals as is
        {
            newRHS += symbol;
        }
        else
        {
            // If terminal already replaced, use existing non-terminal
            if (terminalToNonTerminal.find(symbol) != terminalToNonTerminal.end())
            {
                newRHS += terminalToNonTerminal[symbol];
            }
            else // Replace terminal with new non-terminal
            {
                std::string newNonTerminal = GenerateUniqueNonTerminal();
                m_VN += newNonTerminal; // Add new non-terminal
                m_PRules.insert({ newNonTerminal, std::string(1, symbol) });
                terminalToNonTerminal[symbol] = newNonTerminal; // Map terminal to its new non-terminal
                newRHS += newNonTerminal; // Update RHS
            }
        }
    }
    rhs = newRHS; // Update the original RHS with the modified one
}


void Grammar::BreakDownLongerProductions(const std::string& lhs, const std::string& rhs, std::multimap<std::string, std::string>& newProductions)
{
    std::map<std::string, std::string> seenCombinations; // Track seen RHS combinations to reuse non-terminals

    if (rhs.length() == 2)
    {
        newProductions.insert({ lhs, rhs });
    }
    else if (rhs.length() > 2)
    {
        std::string currentLHS = lhs;
        std::string remainingRHS = rhs;

        while (remainingRHS.length() > 2)
        {
            std::string nextTwoChars = remainingRHS.substr(0, 2);

            // Check if this combination was seen before
            if (seenCombinations.find(nextTwoChars) != seenCombinations.end())
            {
                // Use the existing non-terminal
                newProductions.insert({ currentLHS, remainingRHS.substr(0, 1) + seenCombinations[nextTwoChars] });
                currentLHS = seenCombinations[nextTwoChars];
            }
            else
            {
                std::string newNonTerminal = GenerateUniqueNonTerminal();
                m_VN += newNonTerminal; // Add to non-terminals
                seenCombinations[nextTwoChars] = newNonTerminal; // Track the combination
                newProductions.insert({ currentLHS, remainingRHS.substr(0, 1) + newNonTerminal });
                currentLHS = newNonTerminal;
            }

            remainingRHS = remainingRHS.substr(1);
        }

        newProductions.insert({ currentLHS, remainingRHS }); // Insert the last binary production
    }
}



std::string Grammar::GenerateUniqueNonTerminal() 
{
    static int nonTerminalCount = 0; // Static variable to keep track of the count
    nonTerminalCount++; // Increment count for each new non-terminal
    return "N" + std::to_string(nonTerminalCount); // Return a unique non-terminal
}



