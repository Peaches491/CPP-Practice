#include <thread>

#include "../Boards/Board.h"

class Evaluator {
public:
    virtual void evaluate(Board* board) = 0;
};

class SimpleEvaluator : virtual public Evaluator {
public:
    void evaluate(Board* board);
};

class ThreadedEvaluator : virtual public Evaluator {
private:
    std::vector<std::thread*> threads;
public:
    ThreadedEvaluator(unsigned int thread_count, Board* board);
    ~ThreadedEvaluator();
    void evaluate(Board* board);
};