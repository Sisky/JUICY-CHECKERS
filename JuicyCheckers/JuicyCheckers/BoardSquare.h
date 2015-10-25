/*
 *   Author:  Dave Brown <wpv9142@aut.ac.nz>
 *   Filename: BoardSquare.h
 *   Description:  This class defines each square on the board, each square is comprised of a visual plane
 *				   and also contains an ID, the ID of the piece that occupies it, its occupancy status and 
 *				   some basic getters and setters
 *
 */

#ifndef __BOARDSQUARE_H__
#define __BOARDSQUARE_H__

// each square is aware of ...
// piece class
// occupied
// ID .. this will refer to the ID of the square

// the baord square will contain the ID of the piece that occupies it

// forward declaration
class Piece;

class BoardSquare
{
public:
	BoardSquare(void);
	~BoardSquare(void);

	// occupied
	bool isOccupied() { return m_Occupied; }
	void setOccupied(bool occupied) { m_Occupied = occupied; }

	// piece
	int getPieceID() { return m_PieceID; }
	void setPieceID(int id) { m_PieceID = id; }

	// plane
	void createPlane(int planeNumber);
	Ogre::Plane getPlane() { return m_BoardSquare; }

	// ID
	void setID(int id) { m_ID = id; }
	int getID() { return m_ID; }
	
protected:
private:

// member variables

public:
protected:
	bool m_Occupied;
	int m_ID;
	int m_PieceID;
	Ogre::Plane m_BoardSquare;

private:
};

#endif __BOARDSQUARE_H__