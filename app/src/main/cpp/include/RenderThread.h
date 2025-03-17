#ifndef SHADER_TOY_ANDROID_RENDERTHREAD_H
#define SHADER_TOY_ANDROID_RENDERTHREAD_H

#include <algorithm>
#include <condition_variable>
#include <mutex>
#include <thread>
#include "Renderer.h"

class RenderThread {
public:
    RenderThread();
    ~RenderThread();
    void start();
    void pause();
    void setRenderer(std::shared_ptr<Renderer> renderer);
    void setFrameRate(int frameRate);
    int getFrameRate();
    int getRealFrameRate();

private:
    void run();
    std::unique_ptr<std::thread> mThread;
    std::mutex mMutex;
    std::atomic<bool> mStarted = false;
    std::atomic<bool> mPaused = false;
    std::condition_variable mCv;
    std::shared_ptr<Renderer> mRenderer;
};

#endif // SHADER_TOY_ANDROID_RENDERTHREAD_H