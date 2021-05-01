# Use your machine 

## Introduction
This is an automata application for ALE2 course.
It is an automata version of the LPP's logic UseYourBrain app.

## Current state
Functionalities:
- Parsing files (alphabet, states, finals, transitions)
- Creating Automaton and States.
    -   ```Hash```, ```operator==```, ```operator<```
- Check if input is DFA (finish assignment 1)
- Parse test vector. 
- Check if a word belongs to the automaton (finish assignment 2)
- Display automaton graph.

RegexParser: 
- Tokenizer Finished
- Wait for AST and nodes

AST and nodes:
- Redesign clearly the node pointers/destructors/rules of 3/5/0, etc
- (current reading: just use raw pointers rather than smart pointers)
- Problems now: How to implement IOperator::operate(vector<BaseNode>) ?

Todo: 
- Implement AST and Nodes
- Implement RegexParser
- Handle the case where some test vector fields is not present 
- Parsing regular expression
- Unit Test