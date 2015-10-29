/*
 *   Author:  Dave Brown <wpv9142@aut.ac.nz>
 *   Filename: Board.h
 *   Description:  This class holds an array of boardsquares with some access functions
 *
 */

#ifndef __BOARD_H__
#define __BOARD_H__

// this class is used to define the board containing a vector of boardpieces that can be accessed

// forward declarations
class BoardSquare;

class Board
{
public:
	Board(void);
	~Board(void);

	void addSquare(int id);
	BoardSquare* getSquare(int id);
	BoardSquare* getSquare(Ogre::SceneNode* node);
	int getSquareID(Ogre::SceneNode* node);
	Ogre::SceneNode* getSceneNode(int id, Ogre::SceneManager& sManager); 
	Ogre::SceneNode* getSceneNodeFromPiece(int id, Ogre::SceneManager& sManager);

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