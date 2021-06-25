#ifndef PARSER_H
#define PARSER_H

#include "automaton.h"
#include "state.h"
#include <iostream>
#include <vector>
#include "pushdownautomaton.h"

class Parser
{
private:
    // parsed Automaton
    Automaton avtomat_;
    Automaton *newAvtomat_;

    // Test vector
    std::vector<std::pair<std::string, bool>> testWords_;
    int testIsFinite_;
    int testIsDFA_;


    /* Parsing utilities */
    std::vector<State> ReadStates(std::string input);
    void ReadTransitions(std::istream& is, Transitions& transitions);
    void ReadPushdownTrainsitions(std::istream& is, PushdownTransitions& transitions);
    void ReadTestWords(std::istream& is);


public:
    Parser();

    void ReadFromStream(std::istream& in);

    Automaton getAutomaton();
    std::vector<std::pair<std::string, bool> > getTestWords() const;
    int getTestIsFinite() const;
    int getTestIsDFA() const;

    /* string utilities */
    static inline void ltrim(std::string &s);
    static inline void rtrim(std::string &s);
    static inline void trim(std::string &s);
    Automaton *getNewAvtomat() const;
};

#endif // PARSER_H
