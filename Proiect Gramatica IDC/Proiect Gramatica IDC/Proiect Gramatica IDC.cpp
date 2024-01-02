#include <iostream>

#include "PushDownAutomaton.h"

int main()
{
    std::vector<std::string> Q = { "q0", "q1", "q2" };
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
    bool check= pda.CheckWord("aaabbb");

    if (check == true) std::cout << "TRUE";
    else
        std::cout << "FALSE";
}
