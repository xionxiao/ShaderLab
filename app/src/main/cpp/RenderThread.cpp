#include <atomic>
#include <pthread.h>
#include "RenderThread.h"

RenderThread::RenderThread() {
    mStarted.store(false, std::memory_order_release);
    mThread = std::make_shared<std::thread>(&RenderThread::run, this);
}

void RenderThread::start() {
    mStarted.store(true, std::memory_order_release);
}

void RenderThread::stop() {
    mStarted.store(false, std::memory_order_release);
    if (mThread) {
        mThread->join();
    }
}

void RenderThread::pause() {

}

void RenderThread::run() {
    pthread_setname_np(pthread_self(), "RenderThread");
    while (mStarted.load(std::memory_order_acquire)) {
        // std::this_thread::sleep_for(1000ms);
        std::lock_guard<std::mutex> lock(this->mMutex);
        if (mRenderer) {
            mRenderer->render();
        }
    }
}