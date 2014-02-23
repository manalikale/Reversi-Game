#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#include "ReversiGame.h"
using namespace std;

// print the game board
void printBoard(int board[BOARD_SIZE][BOARD_SIZE]);

// check if a move is valid or not
bool validMove(int board[BOARD_SIZE][BOARD_SIZE], int outBoard[BOARD_SIZE][BOARD_SIZE], int x, int y, int player);

// number of pieces heuristic
int numberOfPieces(int board[BOARD_SIZE][BOARD_SIZE]);

// positional weights heuristic
int positionalWeights(int board[BOARD_SIZE][BOARD_SIZE]);

// minimax algorithm
int miniMax(int board[BOARD_SIZE][BOARD_SIZE],
	int cutOffDepth,
	int maxDepth,
	int playerType,
	evaluation evalFunc,
	int &x,
	int &y,
	bool &canMove);

// alpha beta algorithm
int alphaBeta(int board[BOARD_SIZE][BOARD_SIZE],
	int cutOffDepth,
	int maxDepth,
	int playerType,
	evaluation evalFunc,
	int alpha,
	int beta,
	int &x,
	int &y,
	bool &canMove);

// used to call actual alpha beta algorithm
int alphaBeta(int board[BOARD_SIZE][BOARD_SIZE],
	int cutOffDepth,
	int maxDepth,
	int playerType,
	evaluation evalFunc,
	int &x,
	int &y,
	bool &canMove);

// get the algorithm from code
algorithm getAlgorithm(int algo);

// get the evaluation function from code
evaluation getEvaluation(int algo);

// print out the value
void printValue(int value);

// print log for minimax
void printMinimax(int depth, int bestVal, int x, int y);

// print log for alpha beta
void printAlphaBeta(int depth, int bestVal, int alpha, int beta, int x, int y, bool cut);

int main(int argc, char *argv[])
{
	int task = 1, cutOffDepth = 3;
	string inputFile = "..\\examples\\1\\input1.txt", outputFile = "output1.txt", outputLog = "log.txt";
	for (int i = 0; i < argc; ++i)
	{
		string temp(argv[i]);
		if (temp == "-t")
		{
			task = atoi(argv[i + 1]);
		}
		else if (temp == "-d")
		{
			cutOffDepth = atoi(argv[i + 1]);
		}
		else if (temp == "-i")
		{
			inputFile = argv[i + 1];
		}
		else if (temp == "-op")
		{
			outputFile = argv[i + 1];
		}
		else if (temp == "-ol")
		{
			outputLog = argv[i + 1];
		}
	}

	//cout << "task: " << task << endl;
	//cout << "cutOffDepth: " << cutOffDepth << endl;
	//cout << "inputFile: " << inputFile << endl;
	//cout << "outputFile: " << outputFile << endl;
	//cout << "outputLog: " << outputLog << endl;

	ReversiGame *game = new	ReversiGame(inputFile);
	game->setTask(task);
	game->setAlgorithm(getAlgorithm(task));
	game->setEvalFunc(getEvaluation(task));
	game->setCutOffLength(cutOffDepth);

	game->solve(outputFile, outputLog);

	return 0;
}

void printMinimax(int depth, int bestVal, int x, int y)
{
	if (ReversiGame::stopLog)
	{
		return;
	}
	if (depth == 1)
	{
		cerr << "root " << depth << " ";		
		printValue(bestVal);
		cerr << endl;
	}
	else
	{
		cerr << char(y + 'a') << (x + 1) << " " << depth << " ";
		printValue(bestVal);
		cerr << endl;
	}
}

void printAlphaBeta(int depth, int bestVal, int alpha, int beta, int x, int y, bool cut)
{
	if (ReversiGame::stopLog)
	{
		return;
	}
	if (depth == 1)
	{
		cerr << "root " << depth << " ";
		printValue(bestVal);
		cerr << " ";
		printValue(alpha);
		cerr << " ";
		printValue(beta);
	}
	else
	{
		cerr << char(y + 'a') << (x + 1) << " " << depth << " ";
		printValue(bestVal);
		cerr << " ";
		printValue(alpha);
		cerr << " ";
		printValue(beta);
	}

	if (cut)
	{
		cerr << " CUT-OFF";
	}
	cerr << endl;
}

void printValue(int value)
{
	if (ReversiGame::stopLog)
	{
		return;
	}
	if (value == INF)
	{
		cerr << "Infinity";
	}
	else if (value == -INF)
	{
		cerr << "-Infinity";
	}
	else
	{
		cerr << (value + 0.0f);
	}
}

void printBoard(int board[BOARD_SIZE][BOARD_SIZE])
{
	for (int i = 0; i < BOARD_SIZE; ++i)
	{
		for (int j = 0; j < BOARD_SIZE; ++j)
		{
			if (board[i][j] == 1) cerr << "X ";
			else if (board[i][j] == -1) cerr << "O ";
			else if (board[i][j] == 0) cerr << "* ";
		}
		cerr << endl;
	}
	cerr << endl;
}

bool validMove(int board[BOARD_SIZE][BOARD_SIZE], int outBoard[BOARD_SIZE][BOARD_SIZE], int x, int y, int player)
{
	if (board[x][y] != EMPTY_CELL)
	{
		return false;
	}

	bool valid = false;
	memcpy(outBoard, board, BOARD_SIZE * BOARD_SIZE * 4);

	for (int i = 0; i < DIRECTION; ++i)
	{
		bool metOpponent = false;
		for (int dx = x + direction[i][0], dy = y + direction[i][1]; dx >= 0 && dx < BOARD_SIZE && dy >= 0 && dy < BOARD_SIZE; dx += direction[i][0], dy += direction[i][1])
		{
			if (outBoard[dx][dy] == player)
			{
				if (!metOpponent)
				{
					break;
				}
				else
				{
					valid = true;
					int cellCount = max(abs(dx - x), abs(dy - y));
					for (int count = 0; count < cellCount; ++count)
					{
						outBoard[x + count * direction[i][0]][y + count * direction[i][1]] = player;
					}
					break;
				}
			}
			else if (outBoard[dx][dy] == -player)
			{
				metOpponent = true;
			}
			else if (outBoard[dx][dy] == EMPTY_CELL)
			{
				break;
			}
		}
	}

	return valid;
}

int numberOfPieces(int board[BOARD_SIZE][BOARD_SIZE])
{
	int blackCount = 0, whiteCount = 0;

	for (int i = 0; i < BOARD_SIZE; ++i)
	{
		for (int j = 0; j < BOARD_SIZE; ++j)
		{
			if (board[i][j] == MAX_PLAYER)
			{
				++blackCount;
			}
			else if (board[i][j] == MIN_PLAYER)
			{
				++whiteCount;
			}
		}
	}

	return blackCount - whiteCount;
}

int positionalWeights(int board[BOARD_SIZE][BOARD_SIZE])
{
	int sum = 0;
	for (int i = 0; i < BOARD_SIZE; ++i)
	{
		for (int j = 0; j < BOARD_SIZE; ++j)
		{
			sum += board[i][j] * weightMask[i][j];
		}
	}

	return sum;
}

int miniMax(int board[8][8],
	int cutOffDepth,
	int maxDepth,
	int playerType,
	evaluation evalFunc,
	int &x,
	int &y,
	bool &canMove)
{
	int bestVal = -playerType * INF;

	if (cutOffDepth > maxDepth)
	{
		bestVal = evalFunc(board);
		printMinimax(cutOffDepth, bestVal, x, y);

		return bestVal;
	}


	int nx = 0, ny = 0, ox = x, oy = y;
	bool nCanMove = false;
	bool moved = false;
	int tempBoard[BOARD_SIZE][BOARD_SIZE];
	vector< pair<int, int> > moves;
	for (int i = 0; i < BOARD_SIZE; ++i)
	{
		for (int j = 0; j < BOARD_SIZE; ++j)
		{
			if (validMove(board, tempBoard, i, j, playerType))
			{
				moved = true;
				moves.push_back(make_pair(i, j));
			}
		}
	}

	if (!moved)
	{
		bestVal = evalFunc(board);
	}
	canMove |= moved;

	printMinimax(cutOffDepth, bestVal, x, y);

	for (size_t m = 0; m < moves.size(); ++m)
	{
		int i = moves[m].first;
		int j = moves[m].second;
		if (validMove(board, tempBoard, i, j, playerType))
		{
			nx = i;
			ny = j;
			int value = miniMax(tempBoard, cutOffDepth + 1, maxDepth, -playerType, evalFunc, nx, ny, nCanMove);
			if (value * playerType > bestVal * playerType)
			{
				x = i;
				y = j;
				bestVal = value;
			}

			printMinimax(cutOffDepth, bestVal, ox, oy);
			moved = true;
		}
	}
	
	return bestVal;
}

int alphaBeta(int board[BOARD_SIZE][BOARD_SIZE],
	int cutOffDepth,
	int maxDepth,
	int playerType,
	evaluation evalFunc,
	int alpha,
	int beta,
	int &x,
	int &y,
	bool &canMove)
{
	int bestVal = -playerType * INF;

	if (cutOffDepth > maxDepth)
	{
		bestVal = evalFunc(board);
		printAlphaBeta(cutOffDepth, bestVal, alpha, beta, x, y, false);

		return bestVal;
	}

	int outBoard[BOARD_SIZE][BOARD_SIZE];
	int nx, ny, ox = x, oy = y;
	bool nCanMove = false;
	bool moved = false;
	vector< pair<int, int> > moves;
	for (int i = 0; i < BOARD_SIZE; ++i)
	{
		for (int j = 0; j < BOARD_SIZE; ++j)
		{
			if (validMove(board, outBoard, i, j, playerType))
			{
				moved = true;
				moves.push_back(make_pair(i, j));
			}
		}
	}

	if (!moved)
	{
		bestVal = evalFunc(board);
	}
	canMove |= moved;

	printAlphaBeta(cutOffDepth, bestVal, alpha, beta, x, y, false);

	for (size_t m = 0; m < moves.size(); ++m)
	{
		int i = moves[m].first;
		int j = moves[m].second;
		if (validMove(board, outBoard, i, j, playerType))
		{
			moved = true;
			nx = i;
			ny = j;
			if (playerType == MAX_PLAYER)
			{
				int value = alphaBeta(outBoard, cutOffDepth + 1, maxDepth, -playerType, evalFunc, alpha, beta, nx, ny, nCanMove);
				bool cut = false;
				if (value > alpha)
				{
					alpha = value;
					x = i;
					y = j;

					if (alpha >= beta)
					{
						cut = true;
					}
				}
				if (value > bestVal)
				{
					bestVal = value;
				}

				printAlphaBeta(cutOffDepth, bestVal, alpha, beta, ox, oy, cut);

				if (cut)
				{
					goto out;
				}
			}
			else if (playerType == MIN_PLAYER)
			{
				int value = alphaBeta(outBoard, cutOffDepth + 1, maxDepth, -playerType, evalFunc, alpha, beta, nx, ny, nCanMove);

				bool cut = false;
				if (value < beta)
				{
					beta = value;
					bestVal = value;
					x = i;
					y = j;

					if (alpha >= beta)
					{
						cut = true;
					}
				}
				if (value < bestVal)
				{
					bestVal = value;
				}

				printAlphaBeta(cutOffDepth, bestVal, alpha, beta, ox, oy, cut);

				if (cut)
				{
					goto out;
				}
			}
		}
	}
out:

	return bestVal;
}

int alphaBeta(int board[BOARD_SIZE][BOARD_SIZE],
	int cutOffDepth,
	int maxDepth,
	int playerType,
	evaluation evalFunc,
	int &x,
	int &y,
	bool &canMove)
{
	int alpha = -INF;
	int beta = INF;
	return alphaBeta(board, cutOffDepth, maxDepth, playerType, evalFunc, alpha, beta, x, y, canMove);
}

algorithm getAlgorithm(int algo)
{
	if (algo == 1)
	{
		return miniMax;
	}

	return alphaBeta;
}

evaluation getEvaluation(int algo)
{
	if (algo == 1 || algo == 2)
	{
		return numberOfPieces;
	}

	return positionalWeights;
}
