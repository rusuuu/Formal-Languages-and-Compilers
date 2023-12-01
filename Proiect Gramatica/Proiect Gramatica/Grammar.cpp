#include "Grammar.h"

#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <ctime>


bool Grammar::IsValidGrammar() 
{
    // Check if VN and VT are empty
    if (m_VN.empty() || m_VT.empty()) 
    {
        std::cerr << "VN or VT is empty\n"; // Debugging
        return false;
    }

    // Check if startSymbol exists in VN
    if (m_VN.find_first_of(m_startSymbol) == std::string::npos)
    {
        std::cerr << "Start symbol not in VN\n"; // Debugging
        return false;
    }

    // Check if PRules is empty
    if (m_PRules.empty()) 
    {
        std::cerr << "Production rules are empty\n"; // Debugging
        return false;
    }

    // Check overlap between VN and VT
    if (m_VN.find(m_VT) != std::string::npos) 
    {
        std::cerr << "Overlap between VN and VT\n"; // Debugging
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

    // Read start symbol
    if (std::getline(file, line)) 
    {
        m_startSymbol = Trim(line);
        std::cout << "Start symbol: " << m_startSymbol << "\n";
    }

    // Read non-terminals
    if (std::getline(file, line)) 
    {
        m_VN = Trim(line);
        std::cout << "Non-terminals: " << m_VN << "\n";
    }

    // Read terminals
    if (std::getline(file, line)) 
    {
        m_VT = Trim(line);
        std::cout << "Terminals: " << m_VT << "\n";
    }

    // Read production rules
    std::cout << "Production rules:\n";
    while (std::getline(file, line)) 
    {
        line = Trim(line);
        size_t pos = line.find("->");
        if (pos != std::string::npos) 
        {
            std::string left = Trim(line.substr(0, pos));
            std::string right = Trim(line.substr(pos + 2));
            m_PRules[left] = right;
            std::cout << left << " -> " << right << "\n";
        }
    }
        file.close();
}

Grammar::Grammar(std::string startSymbol, std::string Vn, std::string Vt, std::unordered_map<std::string, std::string> Rules)
    :m_startSymbol(std::move(startSymbol)), m_VN(std::move(Vn)), m_VT(std::move(Vt)), m_PRules(std::move(Rules))
{
}

Grammar::Grammar()
{
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

std::string Grammar::GenerateWord()
{
    if (!IsValidGrammar())
    {
        std::cerr << "Invalid grammar, cannot generate word\n";
        return "";
    }

    std::string currentString = m_startSymbol;
    std::string progress = m_startSymbol; // For tracking progress
    std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr))); // Random number generator

    int maxIterations = 100;
    int iterationCount = 0;

    while (iterationCount < maxIterations && std::any_of(currentString.begin(), currentString.end(), [this](char ch) { return IsNonTerminal(ch); }))
    {
        std::vector<std::pair<std::string, std::string>> applicableProductions;

        // Find all applicable productions
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
            break; // No applicable productions, break the loop
        }

        // Randomly select one production
        std::uniform_int_distribution<int> dist(0, applicableProductions.size() - 1);
        auto selectedProduction = applicableProductions[dist(rng)];

        // Apply the selected production
        size_t pos = currentString.find(selectedProduction.first);
        if (pos != std::string::npos) {
            currentString.replace(pos, selectedProduction.first.length(), selectedProduction.second);
        }

        // Update progress
        progress += " -> " + currentString;

        iterationCount++;
    }

    if (iterationCount >= maxIterations) {
        std::cerr << "Reached maximum iterations, might be stuck in a loop\n";
    }

    std::cout << "The process: " << progress << std::endl;
    return currentString;
}