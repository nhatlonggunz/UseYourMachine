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

    // DFS to check if word belong to automaton
    void ValidateTransitionsInput();

    bool IsWordBelongTo_Util(const State &curState, std::string word, int wordIndex,
                             std::set<std::pair<State, int> > visited);
public:
    Automaton(); // default construtor
    Automaton(std::string alphabet,
              std::vector<State> listStates,
              std::vector<State> listFinalStates_,
              Transitions transitions);


    bool IsDFA() const;
    bool IsWordBelongTo(std::string word); // check if a word belongs to the Automaton
    void ValidateTestVector(bool testIsDFA,
                            bool testIsFinite,
                            std::vector<std::pair<std::string, bool> > testWords);
    std::string ToGraph();

    friend std::ostream& operator<<(std::ostream& os, const Automaton& avtomat);
};



#endif // AUTOMATON_H
