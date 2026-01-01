#include "RenderThread.h"
#include "ALog.h"
#include <atomic>
#include <chrono>
#include <memory>
#include <mutex>
#include <pthread.h>

#define LOG_TAG "RenderThread"

RenderThread::RenderThread() {
    mStarted.store(false, std::memory_order_release);
    mPaused.store(true, std::memory_order_release);
}

RenderThread::~RenderThread() { stop(); }

void RenderThread::start() {
    std::lock_guard<std::mutex> lock(mMutex);
    if (mStarted.load(std::memory_order_acquire)) {
        ALOGW("%s", "RenderThread already started");
        return;
    }

    mStarted.store(true, std::memory_order_release);
    mPaused.store(false, std::memory_order_release);
    mThread = std::make_unique<std::thread>(&RenderThread::run, this);
    ALOGD("%s", "RenderThread started");
}

void RenderThread::stop() {
    {
        std::lock_guard<std::mutex> lock(mMutex);
        if (!mStarted.load(std::memory_order_acquire)) {
            ALOGW("%s", "RenderThread already stopped");
            return;
        }

        mStarted.store(false, std::memory_order_release);
        mPaused.store(false, std::memory_order_release);
        mCv.notify_all();
    }

    if (mThread && mThread->joinable()) {
        mThread->join();
        mThread.reset();
    }
    ALOGD("%s", "RenderThread stopped");
}

void RenderThread::pause() {
    std::lock_guard<std::mutex> lock(mMutex);
    if (!mStarted.load(std::memory_order_acquire)) {
        ALOGW("%s", "RenderThread not started, cannot pause");
        return;
    }

    if (mPaused.load(std::memory_order_acquire)) {
        ALOGW("%s", "RenderThread already paused");
        return;
    }

    mPaused.store(true, std::memory_order_release);
    ALOGD("%s", "RenderThread paused");
}

void RenderThread::resume() {
    std::lock_guard<std::mutex> lock(mMutex);
    if (!mStarted.load(std::memory_order_acquire)) {
        ALOGW("%s", "RenderThread not started, cannot resume");
        return;
    }

    if (!mPaused.load(std::memory_order_acquire)) {
        ALOGW("%s", "RenderThread not paused, cannot resume");
        return;
    }

    mPaused.store(false, std::memory_order_release);
    mCv.notify_all();
    ALOGD("%s", "RenderThread resumed");
}

void RenderThread::setRenderer(std::shared_ptr<Renderer> renderer) {
    std::lock_guard<std::mutex> lock(mMutex);
    if (mRenderer != renderer) {
        mRenderer = renderer;
    }
}

void RenderThread::setFrameRate(int frameRate) {
    // TODO: 实现帧率控制
}

int RenderThread::getFrameRate() {
    // TODO: 实现获取帧率
    return 0;
}

int RenderThread::getRealFrameRate() {
    // TODO: 实现获取实际帧率
    return 0;
}

void RenderThread::run() {
    pthread_setname_np(pthread_self(), "RenderThread");
    ALOGD("%s", "RenderThread running");

    while (mStarted.load(std::memory_order_acquire)) {
        {
            std::unique_lock<std::mutex> lock(mMutex);
            if (mPaused.load(std::memory_order_acquire)) {
                mCv.wait(lock, [this]() {
                    return !mPaused.load(std::memory_order_acquire) ||
                           !mStarted.load(std::memory_order_acquire);
                });
            }

            if (!mStarted.load(std::memory_order_acquire)) {
                break;
            }
        }

        if (mRenderer) {
            mRenderer->render();
        }

        // TODO: 简单的帧率控制
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
    }

    ALOGD("%s", "RenderThread exiting");
}
