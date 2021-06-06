#ifndef PARSER_H
#define PARSER_H

#include "Automata/automaton.h"
#include "state.h"
#include <iostream>
#include <vector>

class Parser
{
private:
    // parsed Automaton
    Automaton avtomat_;

    // Test vector
    std::vector<std::pair<std::string, bool>> testWords_;
    bool testIsFinite_;
    bool testIsDFA_;


    /* Parsing utilities */
    std::vector<State> ReadStates(std::string input);
    void ReadTransitions(std::istream& is, Transitions& transitions);
    void ReadTestWords(std::istream& is);

public:
    Parser();

    void ReadFromStream(std::istream& in);

    Automaton getAutomaton();
    std::vector<std::pair<std::string, bool> > getTestWords() const;
    bool getTestIsFinite() const;
    bool getTestIsDFA() const;

    /* string utilities */
    static inline void ltrim(std::string &s);
    static inline void rtrim(std::string &s);
    static inline void trim(std::string &s);
};

#endif // PARSER_H
