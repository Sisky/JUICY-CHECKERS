/*
*   Author:  Dave Brown <wpv9142@aut.ac.nz>
*   Filename: LineDrawing.h
*   Description:  This is the implementation of the class LineDrawing.cpp that defines a small selection square
*
*/

#ifndef __LINEDRAWING_H__
#define __LINEDRAWING_H__



class LineDrawing
{
public:
	LineDrawing(void);
	~LineDrawing(void);

	void initSelectionSquare(Ogre::SceneManager& sm);

	void setSelectionSquareColor(Ogre::Vector3 col);

	Ogre::ManualObject* getSquare() { return mSquare; }

	enum QueryFlags2
	{
		SELECTION_FLAG = 1 << 6
	};
protected:
private:


	// member variables
public:
	Ogre::ManualObject* mSquare;
protected:
private:
	
};

#endif __LINEDRAWING_H__