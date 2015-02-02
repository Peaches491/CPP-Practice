/*
 * TestClass.cpp
 *
 *  Created on: Jan 30, 2015
 *      Author: Daniel
 */

#include <iostream>
#include <sstream>
#include <cstdio>
#include <cmath>

#include "TestClass.h"


VectorBoard::VectorBoard(int width, int height, bool isToroidal) :
  Board(width, height, isToroidal){
	this->m_width = width;
	this->m_height = height;

	this->buffOne = std::vector<std::vector<bool> >();
	this->buffTwo = std::vector<std::vector<bool> >();

	this->usingBuffTwo = false;
	this->grid = &(this->buffOne);

	for(int buffCount = 0; buffCount < 2; buffCount++) {
		for(int i = 0; i < this->m_width; i++) {
			std::vector<bool> col = std::vector<bool>(this->m_height);
			for(int j = 0; j < this->m_width; j++) {
				col.push_back(false);
			}
			(*(this->grid)).push_back( col );
		}
		this->swapBuffers();
	}
}

VectorBoard::~VectorBoard() {

}

Board::~Board() { }




int VectorBoard::getCell(int x, int y, bool fromBuffer){
	if(this->isToroidal()){
		if(x >= this->getWidth()) {
			x = x % this->getWidth();
		} else if(x < 0) {
			x = this->getWidth() + (x % this->getWidth());
		}

		if(y >= this->getHeight()) {
			y = y % this->getHeight();
		} else if(y < 0) {
			y = this->getHeight() + (y % this->getHeight());
		}
	} else {
		if(x >= this->getWidth()  || x < 0) return false;
		if(y >= this->getHeight() || y < 0) return false;
	}
	if(fromBuffer) {
		return (*(this->getBufferGrid()))[x][y];
	} else {
		return (*(this->getDisplayGrid()))[x][y];
	}
}

bool VectorBoard::setCell(int x, int y, bool value, bool fromBuffer) {
	if(x >= this->getWidth() || x < 0) return false;
	if(y >= this->getWidth() || y < 0) return false;

	if(fromBuffer) {
		this->getBufferGrid()->at(x).at(y) = value;
	} else {
		this->getDisplayGrid()->at(x).at(y) = value;
	}
	return true;
}

std::vector<std::vector<bool> >* VectorBoard::getDisplayGrid() {
	if(this->usingBuffTwo) {
		return &(this->buffTwo);
	} else {
		return &(this->buffOne);
	}
}

std::vector<std::vector<bool> >* VectorBoard::getBufferGrid() {
	if(this->usingBuffTwo) {
		return &(this->buffOne);
	} else {
		return &(this->buffTwo);
	}
}

void VectorBoard::swapBuffers() {
	if(this->usingBuffTwo) {
		this->grid = &(this->buffOne);
	} else {
		this->grid = &(this->buffTwo);
	}
	this->usingBuffTwo = !this->usingBuffTwo;
}

void VectorBoard::initialize(std::vector<std::vector<bool> > centerData) {
	int centerWidth = centerData.size();
	int centerHeight = centerData[0].size();

	int centerX = (this->getWidth()  -  centerWidth)/2.0;
	int centerY = (this->getHeight() - centerHeight)/2.0;

	printf("centerWidth: %d centerHeight: %d ", centerWidth, centerHeight);
	printf("centerX: %d centerY: %d\n", centerX, centerY);

	for(int j = 0; j < centerHeight; j++){
		for(int i = 0; i < centerHeight; i++){
			this->setCell(i+centerX, j+centerY, centerData[i][j]);
		}
	}
}

char VectorBoard::countNeighborsAt(int x, int y) {
	int sum = 0;
	sum += this->getCell(x+1, y  );
	sum += this->getCell(x+1, y+1);
	sum += this->getCell(x,   y+1);
	sum += this->getCell(x-1, y+1);
	sum += this->getCell(x-1, y  );
	sum += this->getCell(x-1, y-1);
	sum += this->getCell(x,   y-1);
	sum += this->getCell(x+1, y-1);
	return sum;
}

void VectorBoard::tick() {
	for(int x = 0; x < this->getWidth(); x++) {
		for(int y = 0; y < this->getHeight(); y++) {
			int numNeighbors = this->countNeighborsAt(x, y);
			if(this->getCell(x, y, false)) {
				this->setCell(x, y, numNeighbors == 2 || numNeighbors == 3, true);
			} else {
				this->setCell(x, y, numNeighbors == 3, true);
			}
		}
	}
	this->swapBuffers();
	this->generation++;
}

void Board::reprint(){

}

void Board::print(){
	printf("Generation: %d\n", this->generation);

	int yLabelWidth = std::ceil(std::log10(getHeight()));
	std::ostringstream stringStream;
	stringStream << "%" << yLabelWidth << "d";

	for(int j = getHeight()-1; j >= 0; j--){
		printf(stringStream.str().c_str(), j);
		for(int i = 0; i < this->getWidth(); i++){
			if(this->getCell(i, j)) {
				printf("\u2588\u2588");
			} else {
//				printf("\u2591\u2591");
				printf(" \u00B7");
			}
		}
		printf("\n");
	}

	stringStream.str(std::string());
	stringStream << "%" << yLabelWidth+2 << "d";
	for(int digitIndex = yLabelWidth-1; digitIndex >= 0; digitIndex--) {
		printf(stringStream.str().c_str(), 0);
		for(int i = 1; i < this->getWidth(); i++) {
			int base =  ((int)std::pow(10, digitIndex));
			int digit = (i / base) % (base*10);
			printf(" %d", digit);
		}
		printf("\n");
	}
	std::cout << std::endl;
}
