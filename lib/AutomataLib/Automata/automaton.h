#ifndef AUTOMATON_H
#define AUTOMATON_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <set>

#include "state.h"

// Transition function is represented here as
// a Hash Table that has:
// - keys as the initial state u,
// - set of StateLink (e,v), representing an edge with input symbol e, to State v

// Hash Tables (unordered set and map) are used for O(1) average-case complexity
// Short, different strings are used as state name and alphabet (A1, A2, a, b, c, ...)
// so collisions cannot happen. Hence O(n) worst-case complexity never occurs.

typedef std::pair<char, State> StateLink; // An edge in the Automaton

struct StateLinkHasher {
    std::size_t operator()(const StateLink& s) const;
};

// Transition function. transition_[startState] = {(symbol, nextState), ...}
typedef std::unordered_map<State, std::unordered_set<StateLink, StateLinkHasher>, StateHasher> Transitions;

const char EMPTY_SYMBOL = '_';

class Automaton
{
private:
    std::string alphabet_;

    // Some processes takes O(n^2)
    // Use Hash Table if number of states ~= 10^5
    // All implementations are generic,
    // hence changing to Hash Table does not require modification of implementation
    std::vector<State> listStates_;

    // Representation:
    // transition_[startState] = {(symbol, nextState),...}
    Transitions transitions_;

    State startState_;
    std::vector<State> listEndStates_;

    // this is used for thompson's construction, which only has one end state
    State oneEndState_;

    // DFS to check if word belong to automaton
    void ValidateTransitionsInput();

    bool IsWordBelongTo_Util(const State &curState, std::string word, int wordIndex,
                             std::set<std::pair<State, int> > visited);

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

public:
    Automaton(); // default construtor
    Automaton(std::string alphabet);
    Automaton(std::string alphabet,
              std::vector<State> listStates,
              std::vector<State> listFinalStates_,
              Transitions transitions);


    /* Automaton high level features: check dfa, to graph... */
    bool IsDFA() const;
    bool IsWordBelongTo(std::string word); // check if a word belongs to the Automaton
    void ValidateTestVector(bool testIsDFA,
                            bool testIsFinite,
                            std::vector<std::pair<std::string, bool> > testWords);

    // Check if the Language represented by this automaton is finite
    // if it is finte, generate all words belonging to the language
    // @return true if the language is finite, false otherwise
    bool ListAllWords(std::vector<std::string>& language);

    std::string ToGraph();
    std::string ToFileContent(std::string comment);

    /* Automaton building */
    void addState(State state);
    void addTransition(State startState, char symbol, State endState);
    void combineAutomaton(Automaton& other);

    friend std::ostream& operator<<(std::ostream& os, const Automaton& avtomat);


    /* Getter and setters */
    std::string alphabet() const;

    std::vector<State> listStates() const;

    Transitions transitions() const;

    State startState() const;
    void setStartState(const State &startState);

    std::vector<State> listEndStates() const;
    void setListEndStates(const std::vector<State> &listEndStates);
    State oneEndState() const;
    void setOneEndState(const State &oneEndState);
};



#endif // AUTOMATON_H
