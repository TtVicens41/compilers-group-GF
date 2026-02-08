# Automata Text File

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
