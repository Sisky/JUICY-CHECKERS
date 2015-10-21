#ifndef __BOARDSQUARE_H__
#define __BOARDSQUARE_H__

// each square is aware of ...
// piece class
// occupied

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

protected:
private:

// member variables

public:
protected:
	bool m_Occupied;
	Piece* m_Piece;
	Ogre::Plane m_BoardSquare;

private:
};

#endif __BOARDSQUARE_H__