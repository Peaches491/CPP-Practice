//============================================================================
// Name        : TestProj.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

//#define _XOPEN_SOURCE 600

//#define SFML_STATIC

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstdio>
#include <ctime>

#include "pthread.h"
#include "TestClass.h"

void* runGame(void *arg) {
	std::cout << "Starting Game!" << std::endl;
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

	VectorBoard *board = new VectorBoard(350, 350, true);
	board->initialize(initGrid);

	int numGenerations = 100;

	std::clock_t start;
	std::clock_t prev;
	double duration;

	start = std::clock();
	prev = std::clock();

	while(board->getCurrentGeneration() < numGenerations) {
		board->tick();
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

//	pthread_barrier_t barr;
//	if(pthread_barrier_init(&barr, NULL, 1)) {
//		printf("Could not create a barrier\n");
//		return -1;
//	}

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
