#include "automaton.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <set>

// StateLinkHasher for HashMap
std::size_t StateLinkHasher::operator()(const StateLink &s) const {
    using std::hash;
    using std::string;

    return hash<char>()(s.first) ^ (hash<string>()(s.second.getName()) << 1);
}

std::string Automaton::alphabet() const
{
    return alphabet_;
}

std::vector<State> Automaton::listStates() const
{
    return listStates_;
}

Transitions Automaton::transitions() const
{
    return transitions_;
}

State Automaton::startState() const
{
    return startState_;
}

void Automaton::setStartState(const State &startState)
{
    startState_ = startState;
}

std::vector<State> Automaton::listEndStates() const
{
    return listEndStates_;
}

void Automaton::setListEndStates(const std::vector<State> &listEndStates)
{
    listEndStates_ = listEndStates;
}

State Automaton::oneEndState() const
{
    return oneEndState_;
}

void Automaton::setOneEndState(const State &oneEndState)
{
    oneEndState_ = oneEndState;
    this->listEndStates_.emplace_back(oneEndState);
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
                throw std::invalid_argument("Transitions contain invalid symbol" + std::string(1, itEndpoints.first));

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

bool Automaton::IsWordBelongTo_Util(const State& curState, std::string word, int wordIndex,
                                    std::set<std::pair<State,int>> visited)
{
    // end of word, end up at a final state.
    if(wordIndex == (int)word.length() && curState.IsFinal())
        return true;

    for(auto&& edge: this->transitions_[curState]) {
        char transSymbol = edge.first;
        State nextState = edge.second;

        std::pair<State,int> nextDfsState(nextState, wordIndex + (transSymbol != EMPTY_SYMBOL));

        if((transSymbol == EMPTY_SYMBOL || transSymbol == word[wordIndex]) &&
                !visited.count(nextDfsState))
        {
            visited.insert(nextDfsState);
            if(IsWordBelongTo_Util(nextDfsState.first, word,
                                   nextDfsState.second, visited))
                return true;
        }
    }

    return false;
}

void Automaton::DfsCheckFiniteLanguage(
        State currentState,
        std::unordered_set<State, StateHasher> &visited,
        std::unordered_set<State, StateHasher> &canReachEnd,
        std::unordered_set<State, StateHasher> &belongsToCycle,
        std::vector<State> &dfsStack)
{
    visited.insert(currentState);
    dfsStack.emplace_back(currentState);

    // current state is end state
    if(std::find(listEndStates_.begin(), listEndStates_.end(), currentState) != listEndStates_.end())
        canReachEnd.insert(currentState);

    for(auto&& links: transitions_[currentState]) {
        State nextState = links.second;

        // tree edge
        if(!visited.count(nextState)) {
            DfsCheckFiniteLanguage(nextState, visited, canReachEnd, belongsToCycle, dfsStack);
        }
        // back edge
        else if(std::find(dfsStack.begin(), dfsStack.end(), State(nextState)) != dfsStack.end()) {
            for(auto it = dfsStack.rbegin(); it != dfsStack.rend(); ++it) {
                belongsToCycle.insert(*it);
                if(it->getName() == nextState.getName())
                    break;
            }
        }

        if(canReachEnd.count(nextState))
            canReachEnd.insert(currentState);
    }

    dfsStack.pop_back();
}

void Automaton::DfsPopulateLanguage(
        State currentState,
        std::unordered_set<State, StateHasher> &visited,
        std::unordered_set<State, StateHasher> &belongsToCycle,
        std::vector<std::string> &language,
        std::string currentWord)
{
    visited.insert(currentState);

    // end state
    if(std::find(listEndStates_.begin(), listEndStates_.end(), currentState) != listEndStates_.end())
    {
        language.push_back(currentWord);
    }

    for(auto&& link: transitions_[currentState]) {
        State nextState = link.second;
        char weight = link.first;

        if(!visited.count(nextState) && !belongsToCycle.count(nextState)) {

            DfsPopulateLanguage(nextState,
                                visited,
                                belongsToCycle,
                                language,
                                currentWord + (weight == EMPTY_SYMBOL? "" : std::string(1, weight)));
        }
    }

    visited.erase(currentState);
}

Automaton::Automaton() {}

Automaton::Automaton(std::string alphabet)
    : alphabet_(alphabet)
{

}

Automaton::Automaton(std::string alphabet,
                     std::vector<State> listStates,
                     std::vector<State> listFinalStates,
                     Transitions transitions)
    : alphabet_(alphabet), listStates_(listStates), transitions_(transitions),
      listEndStates_(listFinalStates)
{
    ValidateTransitionsInput();

    startState_ = listStates_[0];

    // Set states in listState as final states.
    for(auto&& finalState: listFinalStates) {
        auto it = std::find(listStates_.begin(), listStates_.end(), State(finalState));

        if(it == listFinalStates.end())
            throw std::invalid_argument("An input final state does not exist in list of states");
        it->setFinal(true);
    }

    // update final states in transition function
    for(auto&& iter_f: this->transitions_){
        auto trueState = *std::find(listStates_.begin(), listStates_.end(), State(iter_f.first.getName()));
        iter_f.first.setFinal(trueState.IsFinal());

        for(auto&& iter_s: iter_f.second) {
            auto tmpTrueState = *std::find(listStates_.begin(), listStates_.end(), State(iter_s.second.getName()));
            iter_s.second.setFinal(tmpTrueState.IsFinal());
        }
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
    return IsWordBelongTo_Util(startState_, word, 0, std::set<std::pair<State, int>>());
}

void Automaton::ValidateTestVector(bool testIsDFA, bool testIsFinite, std::vector<std::pair<std::string, bool> > testWords)
{
    bool thisIsDFA = IsDFA();

    if(thisIsDFA  != testIsDFA) {
        std::string error = std::string("This automaton is") +
                            (thisIsDFA ? "" : " not") +
                            " DFA. While test vector is" +
                            (testIsDFA ? "" : " not") +
                            " DFA.";

        throw std::invalid_argument(error);
    }
    for(auto&& testWord: testWords) {
        bool belongsToAutomaton = this->IsWordBelongTo(testWord.first);

        if(belongsToAutomaton != testWord.second) {
            std::string error = testWord.first +
                                (belongsToAutomaton ? " belongs to" : " does not belong to") +
                                "  DFA. While test vector says it" +
                                (testWord.second ? " belongs to" : " does not belong to") +
                                " DFA.";
            throw std::invalid_argument(error);
        }
    }
}

bool Automaton::ListAllWords(std::vector<std::string>& language)
{
    std::unordered_set<State, StateHasher> visited;
    std::unordered_set<State, StateHasher> canReachEnd;
    std::unordered_set<State, StateHasher> belongsToCycle;
    std::vector<State> dfsStack;

    DfsCheckFiniteLanguage(this->startState_, visited, canReachEnd, belongsToCycle, dfsStack);

    for(auto&& state: listStates_) {
        if(canReachEnd.count(state) && belongsToCycle.count(state)) {
            return false;
        }
    }

    language.clear();
    visited.clear();
    DfsPopulateLanguage(startState_, visited, belongsToCycle, language);

    std::sort(language.begin(), language.end());
    auto uniqueId = std::unique(language.begin(), language.end());
    language.resize(std::distance(language.begin(), uniqueId));

    return true;
}

std::string Automaton::ToGraph()
{
    // start string
    std::string content = "digraph Automaton {\n"
                          "rankdir=LR;\n"
                          "\"\" [shape=none]\n";

    // end state
    content += "node [shape = doublecircle]; ";
    for(auto&& s : listEndStates_) {
        content += s.getName() + " ";
    }
    content += ";\n";


    // Draw remaining nodes. Draw edges
    content += "node [shape = circle];\n";
    content += std::string("\"\" -> ") + "\"" + startState_.getName() + "\"\n";

    for(auto&& iter_f: transitions_) {
        for(auto&& iter_s: iter_f.second) {
            std::string formattedSymbol = std::string(1, iter_s.first);
            if(formattedSymbol == "_")
                formattedSymbol = "\u03B5";
            content += iter_f.first.getName();
            content += " -> ";
            content += iter_s.second.getName();
            content += std::string("[label = \"") + formattedSymbol + "\"];\n";
        }
    }

    content += "}";

    return content;
}

std::string Automaton::ToFileContent(std::string comment)
{
    std::string content;

    if(comment != "") {
        content += "#" + comment + "\n\n";
    }

    content += "alphabet: " + this->alphabet_ + "\n";

    /* add list states. First state is alway the start state.*/
    content += "states: ";

    content += startState_.getName() + ",";

    for(auto&& state: listStates_)
        if(state.getName() != startState_.getName())
            content += state.getName() + ",";

    content.pop_back();
    content += "\n";

    /* final states */
    content += "final: " + this->oneEndState_.getName() + "\n";

    /* transitions */
    content += "transitions:\n";

    for(auto&& itStart: transitions_) {
        auto start = itStart.first;

        for(auto&& itLink: itStart.second) {
            content += start.getName() + "," +
                       itLink.first + " --> " +
                       itLink.second.getName() + "\n";
        }
    }

    content += "end.";

    return content;
}

void Automaton::addState(State state)
{
    auto itState = std::find(this->listStates_.begin(), this->listStates_.end(), state);

    if(itState != this->listStates_.end())
        return;

    this->listStates_.emplace_back(state);
}

void Automaton::addTransition(State startState, char symbol, State endState)
{
    StateLink link(symbol, endState);

    if(std::find(alphabet_.begin(), alphabet_.end(), symbol) == alphabet_.end() &&
       symbol != EMPTY_SYMBOL)
        throw std::invalid_argument("addTransition: symbol does not exist");
    if(std::find(listStates_.begin(), listStates_.end(), startState) == listStates_.end())
        throw std::invalid_argument("addTransition: start state does not exist");
    if(std::find(listStates_.begin(), listStates_.end(), link.second) == listStates_.end())
        throw std::invalid_argument("addTransition: end state does not exist");

    this->transitions_[startState].insert(link);
}

void Automaton::combineAutomaton(Automaton &other)
{
    if(this->alphabet_ != other.alphabet()) {
        throw std::invalid_argument("Combine Automaton: alphabet does not match");
    }

    // add other's list states to this list states
    for(auto state: other.listStates()) {
        if(std::find(listStates_.begin(), listStates_.end(), state) == listStates_.end())
            this->listStates_.emplace_back(state);
    }

    // move transition from other to this
    for(auto&& itStart: other.transitions()) {
        auto start = itStart.first;

        for(auto&& itLink: itStart.second) {
            this->addTransition(start.getName(), itLink.first, itLink.second.getName());
        }
    }
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
