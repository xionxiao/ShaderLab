#include <atomic>
#include <memory>
#include <mutex>
#include <pthread.h>
#include "RenderThread.h"

RenderThread::RenderThread() {
    mStarted.store(true, std::memory_order_release);
    mPaused.store(true, std::memory_order_release);
    mThread = std::make_unique<std::thread>(&RenderThread::run, this);
}

void RenderThread::start() {
    mPaused.store(false, std::memory_order_release);
    mCv.notify_all();
}

void RenderThread::pause() {
    mPaused.store(false, std::memory_order_release);
}

RenderThread::~RenderThread() {
    mStarted.store(false, std::memory_order_release);
    mPaused.store(false, std::memory_order_release);
    if (mThread) {
        mThread->join();
    }
}

void RenderThread::setRenderer(std::shared_ptr<Renderer> renderer) {
    if (mRenderer != renderer) {
        mRenderer = renderer;
    }
}

void RenderThread::run() {
    pthread_setname_np(pthread_self(), "RenderThread");
    while (mStarted.load(std::memory_order_acquire)) {
        std::unique_lock<std::mutex> lock(this->mMutex);
        if (mPaused.load(std::memory_order_acquire)) {
            mCv.wait(lock);
        }
        if (mStarted.load(std::memory_order_acquire)) {
            break;
        }

        // std::this_thread::sleep_for(1000ms);
        std::lock_guard<std::mutex> auto_lock(this->mMutex);
        if (mRenderer) {
            mRenderer->render();
        }
    }
}