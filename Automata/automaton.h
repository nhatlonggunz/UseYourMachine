#ifndef AUTOMATON_H
#define AUTOMATON_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <functional>

#include "state.h"

// Transition function is represented here as
// a Hash Table that has:
// - keys as the initial state u,
// - set of StateLink (e,v), representing an edge with input symbol e, to State v

// Hash Tables (unordered set and map) are used for O(1) average-case complexity
// Short, different strings are used as state name and alphabet (A1, A2, a, b, c, ...)
// so collisions cannot happen. Hence O(n) worst-case complexity never occurs.

typedef std::pair<char, State> StateLink;

struct StateLinkHasher {
    std::size_t operator()(const StateLink& s) const;
};


typedef std::unordered_map<State, std::unordered_set<StateLink, StateLinkHasher>, StateHasher> Transitions;

const char EMPTY_SYMBOL = '_';

class Automaton
{
private:
    std::string alphabet_;
    std::vector<State> listStates_;
    std::vector<State> listFinalStates_;

    // transition_[startState] = {(symbol, nextState),...}
    Transitions transitions_;

public:
    Automaton();
    Automaton(std::string alphabet,
              std::vector<State> listStates,
              std::vector<State> listFinalStates_,
              Transitions transitions);

    bool IsDFA();

    friend std::ostream& operator<<(std::ostream& os, const Automaton& avtomat);
};



#endif // AUTOMATON_H
