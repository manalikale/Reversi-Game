Reversi-Game
============
Introduction

In this project, I have implemented the Reversi game using Minimax and Alpha-Beta pruning algorithms with two different evaluation functions:
1) number of pieces
2) positional weights.
The rules of the Reversi game can be found e.g. in http://en.wikipedia.org/wiki/Reversi [1]. The interactive examples can be found e.g. http://www.samsoft.org.uk/reversi/[2]. In the Othello version of this game, the game begins with four disks placed in a square in the middle of the grid, two facing light-up, two pieces with the dark side up, with same-colored disks on a diagonal with each other. However, in this assignment, the starting position will be specified in the input file.

Tasks

In this assignment, I have written a program to implement the following algorithms for both min and max players.
2.1 Minimax using number of pieces as an evaluation function 
2.2 Alpha-Beta pruning using number of pieces as an evaluation function 
2.3 Alpha-Beta pruning using positional weights as an evaluation function 

Program execution

reversi -t <task> -d < cutting_off_depth> -i <input_file> -op <output_path> -ol <output_log>
