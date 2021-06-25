#ifndef PUSHDOWNAUTOMATON_H
#define PUSHDOWNAUTOMATON_H

#include "automaton.h"
#include "pushdownstatelink.h"
#include <stack>

typedef std::unordered_map<State,
                           std::set<PushdownStateLink>,
                           StateHasher>
        PushdownTransitions;

class PushDownAutomaton: public Automaton
{
public:
    PushDownAutomaton();
    PushDownAutomaton(std::string alphabet, std::string stackAlphabet);
    PushDownAutomaton(std::string alphabet,
                      std::string stackAlphabet,
                      std::vector<State> listStates,
                      std::vector<State> listFinalStates,
                      PushdownTransitions transitions);

    // The pushdown automaton must be deterministic
    bool IsWordBelongTo(std::string word) override;


    void addTransition(State startState, char symbol, char popSymbol, char pushSymbol, State toState);

private:
    std::string stackAlphabet_;
    PushdownTransitions transitions_;

    bool isWordBelongTo_util(State currentState,
                             int wordIndex,
                             std::string& stack,
                             std::string word,
                             std::set<std::string>& visited);

    void parseVisited(std::string visited,
                      std::string& state,
                      int& wordIndex,
                      std::string& stack);

    bool checkCycle(const State& currentState,
                    int wordIndex,
                    const std::string& stack,
                    const std::set<std::string>& visited);
};

// sort the transitions following precedence rules
bool PushdownTransitionComparer(
        const PushdownStateLink& a,
        const PushdownStateLink& b);

#endif // PUSHDOWNAUTOMATON_H
