#include <iostream>
#include <future>
#include <condition_variable>

#include "Evaluator.h"

std::mutex cvStartMutex;
std::condition_variable condVarStart;
std::mutex cvFinishMutex;
std::condition_variable condVarFinish;

bool should_exit = false;
bool ready = false;
std::atomic<int> threadsComplete = ATOMIC_VAR_INIT(0);



class EvaluateTask {

public:
    EvaluateTask(Board* board, int threadCount) :
            threadNumber(0),
            threadCount(threadCount),
            board(board),
            minX(0),
            maxX(0),
            minY(0),
            maxY(0) {}

    int threadNumber;
    int threadCount;
    Board* board;
    int minX;
    int minY;
    int maxX;
    int maxY;


    void operator()() const
    {

        std::unique_lock<std::mutex> lk(cvStartMutex);
        std::cout << "WAITING - Thread #: " << this->threadNumber << std::endl;

        if(!condVarStart.wait_for(lk, std::chrono::milliseconds(100), []{return ready;})){
            if(should_exit) return;
        }

        std::cout << "Thread #: " << this->threadNumber << std::endl;
        std::cout << "X: " << this->minX << " - " << this->maxX << std::endl;
        std::cout << "Y: " << this->minY << " - " << this->maxY << std::endl;
        std::cout << "Incrementing" << std::endl;
        threadsComplete.fetch_add(1, std::memory_order_relaxed);
        std::cout << "Incremented " << threadsComplete << std::endl;
        std::cout << std::endl;

        if(threadsComplete == threadCount) {
            condVarFinish.notify_all();
            ready = false;
        }
    }
};

ThreadedEvaluator::ThreadedEvaluator(unsigned int thread_count, Board* board){

    for(double j = 0; j < thread_count; j++) {
        for(double i = 0; i < thread_count; i++) {
            EvaluateTask* task = new EvaluateTask(board, thread_count);

            task->threadNumber = (int)((j * thread_count) + i + 1);
            task->threadCount = thread_count * thread_count;
            task->minX = (int)(((i)/thread_count) * board->getWidth());
            task->maxX = (int)(((i+1.0)/thread_count) * board->getWidth());
            task->minY = (int)(((j)/thread_count) * board->getHeight());
            task->maxY = (int)(((j+1.0)/thread_count) * board->getHeight());
            if(i < thread_count-1) {
                task->maxX--;
            }
            if(j < thread_count-1) {
                task->maxY--;
            }

            this->threads.push_back(new std::thread(*task));
        }
    }
}

ThreadedEvaluator::~ThreadedEvaluator() {
    should_exit = true;
    for(std::thread* thread : threads) {
        thread->join();
    }
}

void ThreadedEvaluator::evaluate(Board *board) {
    { // Set threads to work
        std::lock_guard<std::mutex> lk(cvStartMutex);
        ready = true;
    }
    condVarStart.notify_all();

    { // Block until threads complete
        std::unique_lock<std::mutex> lk(cvFinishMutex);

        unsigned long threadCount = threads.size();
        condVarFinish.wait(lk, [threadCount]{
            return threadsComplete == threadCount;
        });

        threadsComplete.store(0, std::memory_order_relaxed);
        lk.release();
    }

    board->iterationComplete();
}

