#ifndef __BOARD_H__
#define __BOARD_H__

// this class is used to define the board containing a vector of boardpieces that can be accessed

#include <vector>

// forward declarations
class BoardSquare;

class Board
{
public:
	Board(void);
	~Board(void);

	void addSquare(int id);
	BoardSquare* getSquare(int id);
protected:
private:
	// member variables
public:
protected:
	// vector array of BoardSquares 
	std::vector<BoardSquare*> m_Board;
private:
};

#endif __BOARD_H__