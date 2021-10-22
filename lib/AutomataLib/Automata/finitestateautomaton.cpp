#include "finitestateautomaton.h"
#include <algorithm>

FiniteStateAutomaton::FiniteStateAutomaton()
{

}

FiniteStateAutomaton::FiniteStateAutomaton(std::string alphabet)
    : Automaton(alphabet)
{

}

FiniteStateAutomaton::FiniteStateAutomaton(
        std::string alphabet,
        std::vector<State> listStates,
        std::vector<State> listFinalStates_,
        Transitions transitions)
    : Automaton(alphabet, listStates, listFinalStates_),
      transitions_(transitions)
{
    this->setStartState(listStates[0]);
}

bool FiniteStateAutomaton::IsDFA() const
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

    return sumDegree ==
            (int)this->listStates().size() * (int)this->alphabet().size();
}

bool FiniteStateAutomaton::IsWordBelongTo(std::string word)
{
    std::set<std::pair<State,int>> visited;
    return IsWordBelongTo_Util(this->startState(), word, 0, visited);
}

void FiniteStateAutomaton::ValidateTestVector(int testIsDFAInput, int testIsFiniteInput, std::vector<std::pair<std::string, bool> > testWords)
{
    if(testIsDFAInput) {
        bool thisIsDFA = IsDFA();
        bool testIsDFA = testIsDFAInput > 0;

        if(thisIsDFA  != testIsDFA) {
            std::string error = std::string("This automaton is") +
                                (thisIsDFA ? "" : " not") +
                                " DFA. While test vector is" +
                                (testIsDFA ? "" : " not") +
                                " DFA.";

            throw std::invalid_argument(error);
        }
    }

    bool testIsFinite = testIsFiniteInput > 0;
    if(testIsFiniteInput) {
        bool isFiniteDFA = true;


        std::unordered_set<State, StateHasher> visited;
        std::unordered_set<State, StateHasher> canReachEnd;
        // note: only contain states belong to a non-empty weight cycle
        std::unordered_set<State, StateHasher> belongsToCycle;
        std::vector<State> dfsStack;
        std::vector<char> weightStack;

        /* Check if the NFA has a finite language */
        DfsCheckFiniteLanguage(this->startState(), visited, canReachEnd, belongsToCycle, dfsStack, weightStack);

        // the language is infinite if:
        // - exist a state can reach a final state
        // - that state belongs to a non-empty weights cycle
        for(auto&& state: this->listStates()) {
            if(canReachEnd.count(state) && belongsToCycle.count(state)) {
                isFiniteDFA = false;
            }
        }


        if(isFiniteDFA  != testIsFinite) {
            std::string error = std::string("This automaton is") +
                                (isFiniteDFA ? "" : " not") +
                                " finite. While test vector is" +
                                (testIsFinite ? "" : " not") +
                                " finite.";

            throw std::invalid_argument(error);
        }
    }

    Automaton::ValidateTestVector(testIsDFAInput, testIsFiniteInput, testWords);
}

bool FiniteStateAutomaton::ListAllWords(std::vector<std::string> &language)
{
    std::unordered_set<State, StateHasher> visited;
    std::unordered_set<State, StateHasher> canReachEnd;
    // note: only contain states belong to a non-empty weight cycle
    std::unordered_set<State, StateHasher> belongsToCycle;
    std::vector<State> dfsStack;
    std::vector<char> weightStack;

    /* Check if the NFA has a finite language */
    DfsCheckFiniteLanguage(this->startState(), visited, canReachEnd, belongsToCycle, dfsStack, weightStack);

    // the language is infinite if:
    // - exist a state can reach a final state
    // - that state belongs to a non-empty weights cycle
    for(auto&& state: this->listStates()) {
        if(canReachEnd.count(state) && belongsToCycle.count(state)) {
            return false;
        }
    }

    /* Populate the language */
    language.clear();
    visited.clear();
    DfsPopulateLanguage(this->startState(), visited, belongsToCycle, language);

    // get unique language
    std::sort(language.begin(), language.end());
    auto uniqueId = std::unique(language.begin(), language.end());
    language.resize(std::distance(language.begin(), uniqueId));

    return true;
}

void FiniteStateAutomaton::addTransition(State startState, char symbol, State endState)
{
    StateLink link(symbol, endState);

    auto alphabet_ = this->alphabet();
    auto listStates_ = this->listStates();

    if(std::find(alphabet_.begin(), alphabet_.end(), symbol) == alphabet_.end() &&
       symbol != EMPTY_SYMBOL)
        throw std::invalid_argument("addTransition: symbol does not exist");
    if(std::find(listStates_.begin(), listStates_.end(), startState) == listStates_.end())
        throw std::invalid_argument("addTransition: start state does not exist");
    if(std::find(listStates_.begin(), listStates_.end(), link.second) == listStates_.end())
        throw std::invalid_argument("addTransition: end state does not exist");

    this->transitions_[startState].insert(link);
}

void FiniteStateAutomaton::combineAutomaton(FiniteStateAutomaton &other)
{
    if(this->alphabet() != other.alphabet()) {
        throw std::invalid_argument("Combine Automaton: alphabet does not match");
    }

    auto listStates_ = this->listStates();

    // add other's list states to this list states
    for(auto& state: other.listStates()) {
        if(std::find(listStates_.begin(), listStates_.end(), state) == listStates_.end())
            this->addState(state);
    }

    // move transition from other to this
    for(auto&& itStart: other.transitions()) {
        auto start = itStart.first;

        for(auto&& itLink: itStart.second) {
            this->addTransition(start.getName(), itLink.first, itLink.second.getName());
        }
    }
}

std::string FiniteStateAutomaton::toGraph()
{
    // start string
    std::string content = "digraph Automaton {\n"
                          "rankdir=LR;\n"
                          "\"\" [shape=none]\n";

    // end state
    content += "node [shape = doublecircle]; ";
    for(auto&& s : this->listEndStates()) {
        content += "\"" + s.getName() + "\" ";
    }
    content += ";\n";


    // Draw remaining nodes. Draw edges
    content += "node [shape = circle];\n";
    content += std::string("\"\" -> ") + "\"" + this->startState().getName() + "\"\n";

    for(auto&& iter_f: transitions_) {
        for(auto&& iter_s: iter_f.second) {
            std::string formattedSymbol = std::string(1, iter_s.first);
            if(formattedSymbol == "_")
                formattedSymbol = "\u03B5";
            content += "\"" + iter_f.first.getName() + "\"";
            content += " -> ";
            content += "\"" + iter_s.second.getName() + "\"";
            content += std::string("[label = \"") + formattedSymbol + "\"];\n";
        }
    }

    content += "}";

    return content;
}

std::string FiniteStateAutomaton::toFileContent(std::string comment)
{
    std::string content;

    if(comment != "") {
        content += "#" + comment + "\n\n";
    }

    content += "alphabet: " + this->alphabet() + "\n";

    /* add list states. First state is alway the start state.*/
    content += "states: ";

    content += this->startState().getName() + ",";

    for(auto&& state: this->listStates())
        if(state.getName() != this->startState().getName())
            content += state.getName() + ",";

    content.pop_back();
    content += "\n";

    /* final states */
    content += "final: " + this->oneEndState().getName() + "\n";

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

Transitions FiniteStateAutomaton::transitions() const
{
    return transitions_;
}


bool FiniteStateAutomaton::IsWordBelongTo_Util(const State &curState, std::string word, int wordIndex, std::set<std::pair<State, int> > &visited)
{
    // end of word, end up at a final state.
    auto listEndStates_ = this->listEndStates();

    if(wordIndex == (int)word.length() &&
       std::find(listEndStates_.begin(), listEndStates_.end(), curState)
            != listEndStates_.end())
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

void FiniteStateAutomaton::DfsCheckFiniteLanguage(State currentState, std::unordered_set<State, StateHasher> &visited, std::unordered_set<State, StateHasher> &canReachEnd, std::unordered_set<State, StateHasher> &belongsToCycle, std::vector<State> &dfsStack, std::vector<char> &weightStack)
{
    visited.insert(currentState);
    dfsStack.emplace_back(currentState);

    // current state is end state
    auto listEndStates_ = this->listEndStates();
    if(std::find(listEndStates_.begin(), listEndStates_.end(), currentState) != listEndStates_.end())
        canReachEnd.insert(currentState);

    for(auto&& links: transitions_[currentState]) {
        State nextState = links.second;

        std::vector<State>::iterator itStartCycle;
        // tree edge
        if(!visited.count(nextState)) {
            weightStack.emplace_back(links.first);
            DfsCheckFiniteLanguage(nextState, visited, canReachEnd, belongsToCycle, dfsStack, weightStack);
            weightStack.pop_back();
        }
        // back edge
        else if((itStartCycle = std::find(dfsStack.begin(), dfsStack.end(), State(nextState))) != dfsStack.end()) {
            bool onlyEmpty = true;

            if(links.first != EMPTY_SYMBOL)
                onlyEmpty = false;

            if(onlyEmpty) {
                unsigned startCycleIndex = itStartCycle - dfsStack.begin();
                for(unsigned i = startCycleIndex; i < weightStack.size(); ++i) {
                    if(weightStack[i] != EMPTY_SYMBOL)
                        onlyEmpty = false;
                }
            }

            if(!onlyEmpty) {
                unsigned startCycleIndex = itStartCycle - dfsStack.begin();
                for(unsigned i = startCycleIndex; i < dfsStack.size(); ++i) {
                    belongsToCycle.insert(dfsStack[i]);
                }
            }
        }

        if(canReachEnd.count(nextState))
            canReachEnd.insert(currentState);
    }

    dfsStack.pop_back();
}

void FiniteStateAutomaton::DfsPopulateLanguage(State currentState, std::unordered_set<State, StateHasher> &visited, std::unordered_set<State, StateHasher> &belongsToCycle, std::vector<std::string> &language, std::string currentWord)
{
    visited.insert(currentState);

    auto listEndStates_ = this->listEndStates();
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
