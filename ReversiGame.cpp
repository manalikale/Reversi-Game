#include "ReversiGame.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <algorithm>
using namespace std;

bool ReversiGame::stopLog = false;

ReversiGame::ReversiGame(string &inputFile)
{
	ifstream fin(inputFile.c_str());

	if (fin.fail())
	{
		exit(1);
	}

	char c;

	for (int i = 0; i < BOARD_SIZE; ++i)
	{
		for (int j = 0; j < BOARD_SIZE; ++j)
		{
			fin >> c;
			if (c == '*')
			{
				mBoard[i][j] = EMPTY_CELL;
			}
			else if (c == 'X')
			{
				mBoard[i][j] = MAX_PLAYER;
			}
			else if (c == 'O')
			{
				mBoard[i][j] = MIN_PLAYER;
			}
		}
	}

	fin.close();
}


ReversiGame::~ReversiGame()
{
}

void ReversiGame::setAlgorithm(algorithm algo)
{
	mAlgorithm = algo;
}

void ReversiGame::setEvalFunc(evaluation eval)
{
	mEvalFunc = eval;
}

void ReversiGame::setTask(int task)
{
	mTask = task;
}

void ReversiGame::setCutOffLength(int cutOffLength)
{
	mCutOffDepth = cutOffLength;
}

int ReversiGame::solve(const string &filename, const string &outputLog)
{
	int stepCount = 0;
	ofstream fout(filename.c_str());
	ofstream ferr(outputLog.c_str());
	cout.rdbuf(fout.rdbuf());
	streambuf *backup = cerr.rdbuf();
	cerr.rdbuf(ferr.rdbuf());
	cerr << std::setprecision(1) << std::fixed;
	int x = -1, y = -1;
	int player = MAX_PLAYER;

	bool canMove = true;
	int passCount = 0;
	int startDepth = 1;

	if (mTask == MINIMAX)
	{
		cerr << "Node,Depth,Value" << endl;
	}
	else
	{
		cerr << "Node,Depth,Value,Alpha,Beta" << endl;
	}

	while (canMove || passCount < 2)
	{
		++stepCount;
		
		x = y = -1;
		canMove = false;
		startDepth = 1;

		memcpy(mOutBoard, mBoard, BOARD_SIZE * BOARD_SIZE * 4);
		int value = mAlgorithm(mBoard, startDepth, mCutOffDepth, player, mEvalFunc, x, y, canMove);
		if (!canMove)
		{
			if (isGameOver())
			{
				cout << "STEP = " << stepCount << endl;
				cout << (player == MAX_PLAYER ? "BLACK" : "WHITE") << endl;
				printBoard(1);
				break;
			}
			++passCount;
			if (passCount >= 2)
			{
				break;
			}
		}
		else
		{
			passCount = 0;
			placePiece(x, y, player);
		}
		
		cout << "STEP = " << stepCount << endl;
		cout << (player == MAX_PLAYER ? "BLACK" : "WHITE") << ((passCount) ? " PASS" : "") << endl;
		printBoard(1);

		player = -player;

		if (stopLog == false)
		{
			stopLog = true;
			ferr.close();
			cerr.rdbuf(backup);
		}
	}

	cout << "Game End" << endl;

	fout.close();
	return stepCount;
}

void ReversiGame::printBoard(int type)
{
	if (type == 0)
	{
		for (int i = 0; i < 8; ++i)
		{
			for (int j = 0; j < 8; ++j)
			{
				if (mBoard[i][j] == 1) cout << "X";
				else if (mBoard[i][j] == -1) cout << "O";
				else if (mBoard[i][j] == 0) cout << "*";
			}
			cout << endl;
		}
		cout << endl;
	}
	else
	{
		for (int i = 0; i < 8; ++i)
		{
			for (int j = 0; j < 8; ++j)
			{
				if (mOutBoard[i][j] == 1) cout << "X";
				else if (mOutBoard[i][j] == -1) cout << "O";
				else if (mOutBoard[i][j] == 0) cout << "*";
			}
			cout << endl;
		}
		cout << endl;
	}
}

bool ReversiGame::placePiece(int x, int y, int player)
{
	if (mBoard[x][y] != EMPTY_CELL)
	{
		return false;
	}

	bool valid = false;

	for (int i = 0; i < DIRECTION; ++i)
	{
		bool metOpponent = false;
		for (int dx = x + direction[i][0], dy = y + direction[i][1]; dx >= 0 && dx < BOARD_SIZE && dy >= 0 && dy < BOARD_SIZE; dx += direction[i][0], dy += direction[i][1])
		{
			if (mBoard[dx][dy] == player)
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
						mBoard[x + count * direction[i][0]][y + count * direction[i][1]] = player;
					}
					break;
				}
			}
			else if (mBoard[dx][dy] == -player)
			{
				metOpponent = true;
			}
			else if (mBoard[dx][dy] == EMPTY_CELL)
			{
				break;
			}
		}
	}

	return valid;
}

bool ReversiGame::isGameOver()
{
	for (int i = 0; i < BOARD_SIZE; ++i)
	{
		for (int j = 0; j < BOARD_SIZE; ++j)
		{
			if (mBoard[i][j] == EMPTY_CELL)
			{
				return false;
			}
		}
	}

	return true;
}

