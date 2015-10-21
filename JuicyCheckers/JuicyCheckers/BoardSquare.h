#ifndef __BOARDSQUARE_H__
#define __BOARDSQUARE_H__

// each square is aware of ...
// piece class
// occupied
// ID .. this will refer to the ID of the square

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
	Piece* getPiece() { return m_Piece; }
	void setPiece(Piece* piece) { m_Piece = piece; }

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
	Piece* m_Piece;
	Ogre::Plane m_BoardSquare;

private:
};

#endif __BOARDSQUARE_H__