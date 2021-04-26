#include "automaton.h"
#include <algorithm>
#include <iostream>
#include <string>

std::size_t StateLinkHasher::operator()(const StateLink &s) const {
    using std::hash;
    using std::string;

    return hash<char>()(s.first) ^ (hash<string>()(s.second.getName()) << 1);
}

void Automaton::ValidateTransitionsInput()
{
    // check if starting state is a valid state
    for(auto& it: transitions_)
    {
        auto stateInList = std::find(listStates_.begin(), listStates_.end(), State((it.first).getName()));

        if(stateInList == listStates_.end()) {
            throw std::invalid_argument("Transitions contain non-exist initial state");
        }

        auto setEnpoints = it.second;

        for(auto &itEndpoints: setEnpoints)
        {
            // check if transition symbol is a valid symbol
            if(this->alphabet_.find(std::string(1, itEndpoints.first)) == std::string::npos &&
               itEndpoints.first != EMPTY_SYMBOL)
                throw std::invalid_argument("Transitions contain invalid symbol");

            // check if next state is a valid state
            stateInList = std::find(listStates_.begin(),
                                    listStates_.end(),
                                    State((itEndpoints.second).getName()));

            if(stateInList == listStates_.end()) {
                throw std::invalid_argument("Transitions contain non-exist next state");
            }
        }
    }
}

Automaton::Automaton() {}

Automaton::Automaton(std::string alphabet,
                     std::vector<State> listStates,
                     std::vector<State> listFinalStates,
                     Transitions transitions)
    : alphabet_(alphabet), listStates_(listStates), transitions_(transitions)
{
    ValidateTransitionsInput();

    // Set states in listState as final states.
    for(auto& finalState: listFinalStates) {
        auto it = std::find(listStates_.begin(), listStates_.end(), State(finalState));

        if(it == listFinalStates.end())
            throw std::invalid_argument("An input final state does not exist in list of states");
        it->setFinal(true);
    }
}


// Every state must have |alphabet| ingoing and outgoing edges.
// Hence a DFA has |alphabet| * |state|,
// given that there is no "_" (epsilon)
bool Automaton::IsDFA() const
{
    int sumDegree = 0;

    for(auto&& iter: transitions_) {
        State start = iter.first;

        auto setLinks = iter.second;

        // check if exist an edge with symbol "_"
        auto EmptyLink = std::find_if(setLinks.begin(), setLinks.end(),
                                      [](const StateLink& s) { return s.first == EMPTY_SYMBOL; });
        if(EmptyLink != setLinks.end())
            return false;

        sumDegree += setLinks.size();
    }

    return sumDegree == (int)listStates_.size() * (int)alphabet_.size();
}

bool Automaton::IsWordBelongTo(std::string word)
{

}

std::ostream& operator<<(std::ostream& os, const Automaton& avtomat)
{
    std::string output = "";
    output += avtomat.alphabet_ + "\n";
    output += "\n";

    for(auto&& state: avtomat.listStates_) {
        output += state.getName() + ",";
    }
    output.erase(output.size() - 1, 1); // remove last ","
    output += "\n";
    output += "\n";


    for(auto&& state: avtomat.listStates_) {
        if(state.IsFinal())
            output += state.getName() + ",";
    }
    output.erase(output.size() - 1, 1); // remove last ","
    output += "\n";
    output += "\n";

    for(auto&& s_iter: avtomat.transitions_)
    {
        State start = s_iter.first;
        auto setLinks = s_iter.second;

        for(auto& link: setLinks) {
            output += start.getName() +
                      "," +
                      std::string(1, link.first) +
                      "-->" +
                      (link.second).getName() +
                      "\n";
        }
    }

    output += "\n";
    output += "\n";

    output += "Is DFA: ";
    output += (avtomat.IsDFA()? "y" : "n");

    os << output;

    return os;
}
