#include "parser.h"
#include <algorithm>
#include <regex>
#include <string>

#include "state.h"

Parser::Parser()
{

}

void Parser::ReadFromStream(std::istream &in)
{
    std::string alphabet;
    std::vector<State> listStates;
    std::vector<State> listFinalStates;
    Transitions transitions;

    std::string line;

    while(std::getline(in, line))
    {
        // preprocess string
        line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
        line.erase(std::remove(line.begin(), line.end(), '\t'), line.end());
        trim(line);

        if(line.size() == 0 || line[0] == '#')
            continue;

        if(std::regex_search(line, std::regex("^alphabet:"))) {
            std::smatch sm;
            std::regex_search(line, sm, std::regex("[^:]*$"));
            alphabet = sm[0];
        }
        if(std::regex_search(line, std::regex("^states:"))) {
            std::smatch sm;
            std::regex_search(line, sm, std::regex("[^:]*$"));

            std::string input;
            input = sm[0];

            listStates = ReadStates(input);
        }
        if(std::regex_search(line, std::regex("^final:"))) {
            std::smatch sm;
            std::regex_search(line, sm, std::regex("[^:]*$"));

            std::string input;
            input = sm[0];

            listFinalStates = ReadStates(input);
        }
        if(std::regex_search(line, std::regex("^transitions:"))) {
            ReadTransitions(in, transitions);
        }
        if(line.size() == 5 && line.substr(0, 3) == "dfa") {
            char tmp = line[4]; // format is "dfa:y"
            this->testIsDFA_ = (tmp == 'y');
        }
        if(line.size() == 8 && line.substr(0, 6) == "finite") {
            char tmp = line[7]; // finite:n
            this->testIsFinite_ = (tmp == 'y');
        }
        if(line.size() == 6 && line == "words:") {
            ReadTestWords(in);
        }
    }

    avtomat_ = Automaton(alphabet, listStates, listFinalStates, transitions);
}

// trim from start (in place)
inline void Parser::ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
inline void Parser::rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
inline void Parser::trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

Automaton Parser::getAutomaton()
{
    return avtomat_;
}

std::vector<std::pair<std::string, bool> > Parser::getTestWords() const
{
    return testWords_;
}

bool Parser::getTestIsFinite() const
{
    return testIsFinite_;
}

bool Parser::getTestIsDFA() const
{
    return testIsDFA_;
}

std::vector<State> Parser::ReadStates(std::string input)
{
    size_t pos;
    std::vector<State> listTokens;

    while((pos = input.find(",")) != std::string::npos) {
        listTokens.emplace_back(State(input.substr(0, pos)));
        input.erase(0, pos + 1);
    }
    // add last token, which does not have ","
    listTokens.emplace_back(State(input.substr(0, input.size())));

    return listTokens;
}

void Parser::ReadTransitions(std::istream& is, Transitions& transitions)
{
    std::string line;

    while(std::getline(is, line))
    {
        // preprocess string
        line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
        line.erase(std::remove(line.begin(), line.end(), '\t'), line.end());
        trim(line);

        if(line.size() == 0 || line[0] == '#')
            continue;
        if(line == "end.")
            break;

        std::smatch sm;
        std::regex_search(line, sm, std::regex("^([A-Za-z0-9]+),"));
        std::string str = sm[1]; // State name
        State start = State(str);

        std::regex_search(line, sm, std::regex("^[A-Za-z0-9]+,([a-z_])-->"));
        str = sm[1]; // transition symbol
        char symbol = str[0];

        std::regex_search(line, sm, std::regex("^[A-Za-z0-9]+,[a-z_]-->([A-Za-z0-9]+)$"));
        str = sm[1]; // Destination state
        State dest = State(str);

        transitions[start].insert(StateLink(symbol, dest));
    }
}

void Parser::ReadTestWords(std::istream &is)
{
    std::string line;

    while(std::getline(is, line)) {
        // preprocess string
        line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
        line.erase(std::remove(line.begin(), line.end(), '\t'), line.end());
        trim(line);

        if(line.size() == 0 || line[0] == '#')
            continue;
        if(line == "end.")
            break;

        std::string tmpWord = line.substr(0, line.find(","));
        char tmpIsValid = line.back();

        if(tmpWord.size() + 2 != line.size()) {
            throw std::invalid_argument("A word is valid");
        }

        this->testWords_.push_back(std::make_pair(tmpWord, tmpIsValid == 'y'));
    }
}
