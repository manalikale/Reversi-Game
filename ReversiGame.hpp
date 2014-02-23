#ifndef REVERSI_GAME_H
#define REVERSI_GAME_H

#include <cstring>
#include <iostream>
using namespace std;

#define MAX_PLAYER 1
#define EMPTY_CELL 0
#define MIN_PLAYER -1
#define BOARD_SIZE 8
#define INF 1000000
#define DIRECTION 8
#define MINIMAX 1
#define ALPHA_BETA_PIECE 2
#define ALPHA_BETA_WEIGHT 3

typedef int(*evaluation)(int board[8][8]);
typedef int(*algorithm)(int board[8][8], int cutOffDepth, int maxDepth, int playerType, evaluation evalFunc, int &x, int &y, bool &canMove);

const int weightMask[BOARD_SIZE][BOARD_SIZE] = { { 99, -8, 8, 6, 6, 8, -8, 99 },
{ -8, -24, -4, -3, -3, -4, -24, -8 },
{ 8, -4, 7, 4, 4, 7, -4, 8 },
{ 6, -3, 4, 0, 0, 4, -3, 6 },
{ 6, -3, 4, 0, 0, 4, -3, 6 },
{ 8, -4, 7, 4, 4, 7, -4, 8 },
{ -8, -24, -4, -3, -3, -4, -24, -8 },
{ 99, -8, 8, 6, 6, 8, -8, 99 } };

const int direction[DIRECTION][2] = { { 0, 1 }, { 0, -1 }, { 1, 0 }, { -1, 0 }, { 1, 1 }, { 1, -1 }, { -1, 1 }, { -1, -1 } };

class ReversiGame
{
private:
	int mBoard[8][8];		// the game board
	int mOutBoard[8][8];	// output board for one step
	algorithm mAlgorithm;	// 1 = minimax, 2 = alpha-beta
	evaluation mEvalFunc;	// evaluation function
	int mCutOffDepth;		// max depth to search to
	int mTask;				// task code: 1, 2, 3

	bool placePiece(int x, int y, int player);	// try place a piece at (x, y) and compute the score for that player
	bool isGameOver();		// check if the game is over or not
public:
	ReversiGame(string &inputFile);
	~ReversiGame();

	static bool stopLog;

	void printBoard(int type);

	// set the algorithm: minimax or alpha-beta
	void setAlgorithm(algorithm algo);

	// set evaluation function: numer of pieces or positional weights
	void setEvalFunc(evaluation evalFunc);

	// set cut off length
	void setCutOffLength(int cutOffLength);

	// set task
	void setTask(int task);

	// solve the game, default algorithm is minimax with number of pieces
	int solve(const string &outputFile, const string &outputLog);
};

#endif
