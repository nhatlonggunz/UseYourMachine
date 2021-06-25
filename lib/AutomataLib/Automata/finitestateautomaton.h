#ifndef FINITESTATEAUTOMATON_H
#define FINITESTATEAUTOMATON_H

#include "automaton.h"

class FiniteStateAutomaton : public Automaton
{
public:
    FiniteStateAutomaton();
    FiniteStateAutomaton(std::string alphabet,
                         std::vector<State> listStates,
                         std::vector<State> listFinalStates_,
                         Transitions transitions);

    bool IsDFA() const;
    bool IsWordBelongTo(std::string word) override;

    void ValidateTestVector(bool testIsDFA,
                            bool testIsFinite,
                            std::vector<std::pair<std::string, bool> > testWords);

    /* Automaton building */
    void addTransition(State startState, char symbol, State endState);
    void combineAutomaton(FiniteStateAutomaton& other);

    Transitions transitions() const;

private:
    Transitions transitions_;

    bool IsWordBelongTo_Util(
            const State &curState,
            std::string word,
            int wordIndex,
            std::set<std::pair<State, int> >& visited);

    void DfsCheckFiniteLanguage(State currentState,
                                std::unordered_set<State, StateHasher>& visited,
                                std::unordered_set<State, StateHasher>& canReachEnd,
                                std::unordered_set<State, StateHasher>& belongsToCycle,
                                std::vector<State>& dfsStack,
                                std::vector<char>& weightStack);
};

#endif // FINITESTATEAUTOMATON_H
