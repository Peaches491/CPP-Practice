/*
 * Board.cpp
 *
 *  Created on: Jan 30, 2015
 *      Author: Daniel
 */

#include <iostream>
#include <sstream>
#include <cstdio>
#include <cmath>

#include "Board.h"


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
