#ifndef NFATODFACONVERTER_H
#define NFATODFACONVERTER_H

#include "automaton.h"

/*
State naming convention: "s0,s1,s2,s3,s4,s10"
*/
class NFAToDFAConverter
{
public:
    NFAToDFAConverter();
    NFAToDFAConverter(Automaton nfa);

    std::vector<State> findClosure(State nfaState);


private:
    Automaton nfa_;
    std::unordered_map<State, std::vector<State>, StateHasher> closures_;
    std::unordered_set<State, StateHasher> dfaStates_;

    State createDfaState(std::vector<State> nfaStates);
    std::vector<State> dfaStateToNFAStates(State dfaState);
};

#endif // NFATODFACONVERTER_H
