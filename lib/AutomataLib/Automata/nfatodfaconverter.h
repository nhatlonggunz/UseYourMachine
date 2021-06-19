#ifndef NFATODFACONVERTER_H
#define NFATODFACONVERTER_H

#include "automaton.h"
#include <queue>

/*
State naming convention: "s0,s1,s2,s3,s4,s10"
*/
class NFAToDFAConverter
{
public:
    NFAToDFAConverter();
    NFAToDFAConverter(Automaton nfa);


    Automaton getDFA();

private:
    Automaton nfa_;
    std::unordered_map<State, std::vector<State>, StateHasher> closures_;
    std::unordered_set<State, StateHasher> dfaStates_;

    State createDfaState(std::vector<State> nfaStates);
    std::vector<State> dfaStateToNFAStates(State dfaState);
    std::vector<State> findClosure(State nfaState);

    void exploreState(State curState, Automaton& dfa, std::queue<State>& qu);
    Automaton ConvertToDFA();

};

bool NFAToDFAStateComparer(State a, State b);

#endif // NFATODFACONVERTER_H
