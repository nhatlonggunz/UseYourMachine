#ifndef NFATODFACONVERTER_H
#define NFATODFACONVERTER_H

#include "finitestateautomaton.h"
#include <queue>

/*
State naming convention: "s0,s1,s2,s3,s4,s10"
*/
class NFAToDFAConverter
{
public:
    NFAToDFAConverter();
    NFAToDFAConverter(FiniteStateAutomaton* nfa);


    FiniteStateAutomaton getDFA();

private:
    FiniteStateAutomaton* nfa_;
    std::unordered_map<State, std::vector<State>, StateHasher> closures_;
    std::unordered_set<State, StateHasher> dfaStates_;


    State createDfaState(std::vector<State> nfaStates);
    std::vector<State> dfaStateToNFAStates(State dfaState);
    std::vector<State> findClosure(State nfaState);

    void exploreState(State curState, FiniteStateAutomaton& dfa, std::queue<State>& qu);
    FiniteStateAutomaton ConvertToDFA();

};

bool NFAToDFAStateComparer(State a, State b);

#endif // NFATODFACONVERTER_H
