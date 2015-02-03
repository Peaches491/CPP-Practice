//============================================================================
// Name        : main.cpp
// Author      : Daniel Miller
// Version     : 0.0.1
// Copyright   : Your copyright notice
// Description : Multi-threaded Game of Life implementation
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstdio>
#include <ctime>
#include <pthread.h>

#include "Evaluators/Evaluator.h"
#include "Boards/Board.h"


void* runGame(void *arg) {
	std::vector<std::vector<bool> > initGrid;

	std::vector<bool> col;
	col.push_back(false);
	col.push_back(true);
	col.push_back(false);
	initGrid.push_back(col);

	col = std::vector<bool>();
	col.push_back(true);
	col.push_back(true);
	col.push_back(true);
	initGrid.push_back(col);

	col = std::vector<bool>();
	col.push_back(false);
	col.push_back(false);
	col.push_back(true);
	initGrid.push_back(col);

	VectorBoard *board = new VectorBoard(35, 35, true);
	board->initialize(initGrid);

	int numGenerations = 100;

	std::clock_t start;
	std::clock_t prev;
	double duration;

	start = std::clock();
	prev = std::clock();

    Evaluator *eval = new ThreadedEvaluator(2, board);

	while(board->getCurrentGeneration() < numGenerations) {
		eval->evaluate(board);
		std::cout << "Generation: "
				  << board->getCurrentGeneration()
				  << "  Duration: "
				  << ((std::clock()-prev)/ (double) CLOCKS_PER_SEC)
				  << std::endl;

		prev = std::clock();
	}

	duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
	std::cout << "Avg. Speed: "<< duration/numGenerations <<'\n';

	return 0;
}

int main(void) {
	printf( "Hello World!!! \n\n" );
	fflush(stdout);

	pthread_t* thr = (pthread_t*)malloc(sizeof(pthread_t));
	pthread_attr_t threadAttr;
	size_t threadStackSize = 65536;
	pthread_attr_init(&threadAttr);
	pthread_attr_setstacksize(&threadAttr,threadStackSize);

	int err = pthread_create(thr, &threadAttr, &runGame, 0);
	std::cout << "Error code: " << err << std::endl;

	pthread_join(*thr, 0);

	return EXIT_SUCCESS;
}
