#include "Gramatica.h"
#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

bool Gramatica::IsValidGrammar()
{
    //sa nu fie vn si vt goale
    if (m_VN.empty() || m_VT.empty()) return false;

    //sa existe startSymbolul in Vn
    if (std::find(m_VN.begin(), m_VN.end(), m_startSymbol) == m_VN.end()) return false;

   
    if (m_PRules.empty()) return false;

    //nu se suprapuna el din vt si vn
    if (m_VN.find(m_VT) != std::string::npos)
    {
        return false;
    }

    return true;
}

void Gramatica::ReadGrammar(const std::string& filename) 
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
        std::istringstream iss(line);
        iss >> m_startSymbol; 
        std::string symbol;
        while (iss >> symbol)
        {
            m_VN += symbol;  
        }
    }

    
    if (std::getline(file, line)) 
    {
        m_VT = line;  
    }

    
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string left, right;
        if (!(iss >> left >> right)) 
        { 
            break; 
        } 

        m_PRules[left] = right;
    }

    file.close();
}


Gramatica::Gramatica(std::string startSymbol, std::string Vn, std::string Vt, std::unordered_map<std::string, std::string> Rules)
    :m_startSymbol(startSymbol), m_VN(Vn), m_VT(Vt), m_PRules(Rules)
{
}

bool Gramatica::IsRegular()
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

bool Gramatica::IsNonTerminal(const char character)
{
    return m_VN.find(character) != std::string::npos;
}

bool Gramatica::IsTerminalString(const std::string& inputString)
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
