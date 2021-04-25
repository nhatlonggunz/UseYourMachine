#ifndef PARSER_H
#define PARSER_H

#include "Automata/automaton.h"
#include "state.h"
#include <iostream>
#include <vector>

class Parser
{
public:
    Parser();

    static Automaton ReadFromStream(std::istream& in);

    /* Parsing utilities */
    static std::vector<State> ReadStates(std::string input);
    static void ReadTransitions(std::istream& is, Transitions& transitions);

    /* string utilities */
    static inline void ltrim(std::string &s);
    static inline void rtrim(std::string &s);
    static inline void trim(std::string &s);
};

#endif // PARSER_H
