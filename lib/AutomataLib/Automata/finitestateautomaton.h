#ifndef FINITESTATEAUTOMATON_H
#define FINITESTATEAUTOMATON_H

#include "automaton.h"

class FiniteStateAutomaton : public Automaton
{
public:
    FiniteStateAutomaton();
    FiniteStateAutomaton(std::string alphabet);
    FiniteStateAutomaton(std::string alphabet,
                         std::vector<State> listStates,
                         std::vector<State> listFinalStates_,
                         Transitions transitions);

    bool IsDFA() const override;
    bool IsWordBelongTo(std::string word) override;

    void ValidateTestVector(int testIsDFAInput, int testIsFiniteInput,
                                    std::vector<std::pair<std::string, bool> > testWords) override;

    bool ListAllWords(std::vector<std::string>& language) override;

    /* Automaton building */
    void addTransition(State startState, char symbol, State endState);
    void combineAutomaton(FiniteStateAutomaton& other);

    std::string toGraph() override;
    std::string toFileContent(std::string comment) override;

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

    void DfsPopulateLanguage(State currentState,
                             std::unordered_set<State, StateHasher>& visited,
                             std::unordered_set<State, StateHasher>& belongsToCycle,
                             std::vector<std::string>& language,
                             std::string currentWord = "");
};

#endif // FINITESTATEAUTOMATON_H
