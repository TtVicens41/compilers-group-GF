# Practice 2: Lexical Analysis

Created by Marc Bosch Manzano<br>
Date of creation 2026/02/09

## How to start?

In a Unix Operating System, execute `./build_local.sh`. It will compile our lexer program. Then, execute the compiled `./lexer`. At the moment, the full program implementation is not given, but we have implemented the automatas and we have checked they work fine.


## Automata Text File

Each automaton in automata text file is structured as following.
```
1. unique symbols of the alfabet as characters
2. number of symbols of the alfabet as integer
3. number of states as integer 
4. initial state as integer
5. accepting states as integer list
6. transition function as integer matrix
```

Each automaton is separated by an extra line jump character.

An example of automaton:
```
if
2
4
1
3
0 0
2 0
0 3
0 0
```
