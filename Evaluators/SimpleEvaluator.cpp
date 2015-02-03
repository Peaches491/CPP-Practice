#include "Evaluator.h"

void SimpleEvaluator::evaluate(Board* board) {
    for(int x = 0; x < board->getWidth(); x++) {
        for(int y = 0; y < board->getHeight(); y++) {
            int numNeighbors = board->countNeighborsAt(x, y);
            if(board->getCell(x, y, false)) {
                board->setCell(x, y, numNeighbors == 2 || numNeighbors == 3, true);
            } else {
                board->setCell(x, y, numNeighbors == 3, true);
            }
        }
    }
    board->iterationComplete();
}
