# About
* C/C++ implementation of Crochemore's Repetition Algorithm

A repetition in a string is described as at least two consecutive identical factors e.g. _abab_ is a repetition in _aababba_. 
Crochemore's algorithm computes for a word _X_ in the set _A*_, where _A_ is the alphabet and _A*_ is the free monoid generated by A, the set of repetitions or the maximal repetition. 
The set of free monoinoids represents all the possible combination of letters from the alphabet A.
This implementation currently looks at finding the set of repetition of a string.
### How to use:
#### Serial implementation:
* clone repo
* go to /src dir
* gcc-6 -o serep serep.c
* ./serep *input* 
#### Parallel implementation:
* clone repo
* go to /src dir
* gcc-6 -o prep prep.c
* ./prep *input* 
