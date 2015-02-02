/*
 * TestClass.h
 *
 *  Created on: Jan 30, 2015
 *      Author: Daniel
 */

#ifndef GAMEOFLIFE_TESTCLASS_H_
#define GAMEOFLIFE_TESTCLASS_H_

#include <stdio.h>
#include <string>
#include <vector>


class Board {
protected:
	int m_width;
	int m_height;
	bool m_isToroidal;
	int generation;

public:
	Board(int width, int height, bool isToroidal) :
		m_width(width),
		m_height(height),
		m_isToroidal(isToroidal),
		generation(0){ };
	virtual ~Board();

	int getWidth() {return m_width;};
	int getHeight() {return m_height;};
	int isToroidal() {return m_isToroidal;};
	int getCurrentGeneration() {return generation;};

	virtual void initialize(std::vector<std::vector<bool> > centerData) = 0;
	virtual char countNeighborsAt(int x, int y) = 0;
	virtual void tick() = 0;

	virtual int getCell(int x, int y, bool fromBuffer=false) = 0;
	virtual bool setCell(int x, int y, bool value, bool fromBuffer=false) = 0;

	void print();
	void reprint();
};


class VectorBoard : virtual public Board {
private:
	bool usingBuffTwo;
	std::vector<std::vector<bool> >* grid;
	std::vector<std::vector<bool> > buffOne;
	std::vector<std::vector<bool> > buffTwo;
protected:
	void swapBuffers();
public:
	VectorBoard(int width, int height, bool isToroidal);
	~VectorBoard();

	void initialize(std::vector<std::vector<bool> > centerData);
	char countNeighborsAt(int x, int y);
	void tick();

	std::vector<std::vector<bool> >* getDisplayGrid();
	std::vector<std::vector<bool> >* getBufferGrid();

	int getCell(int x, int y, bool fromBuffer=false);
	bool setCell(int x, int y, bool value, bool fromBuffer=false);
};


#endif /* GAMEOFLIFE_TESTCLASS_H_ */
