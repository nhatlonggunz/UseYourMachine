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
- Done

AST and nodes:
- Redesign clearly the node pointers/destructors/rules of 3/5/0, etc

- Problem: How to implement IOperator::operate(vector<BaseNode>)
	(current reading: just use raw pointers rather than smart pointers)
	(done: use pointers)

- Problem: Should regex parser generate AST? 
	(done: regex parser only tokenizes. AST build the tree from tokens. 
		otherwise, regexparser knows AST completely, violating encapsulation.

- Build tree done
- To string done
- Convert Regex to NFA using Thompson's Construction.

Todo: 
- Handle the case where some test vector fields is not present 
- Unit Test