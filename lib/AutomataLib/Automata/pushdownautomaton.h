#ifndef PUSHDOWNAUTOMATON_H
#define PUSHDOWNAUTOMATON_H

#include "automaton.h"
#include "pushdownstatelink.h"

typedef std::unordered_map<State,
                           std::unordered_set<PushdownStateLink, PushdownStateLinkHasher>,
                           StateHasher>
        PushdownTransitions;

class PushDownAutomaton: public Automaton
{
public:
    PushDownAutomaton();
    PushDownAutomaton(std::string alphabet, std::string stackAlphabet);

    void addTransition(State startState, char symbol, char popSymbol, char pushSymbol, State toState);

private:
    PushdownTransitions transitions_;
    std::string stackAlphabet_;
};

#endif // PUSHDOWNAUTOMATON_H
