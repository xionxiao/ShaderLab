#ifndef SHADER_TOY_ANDROID_RENDERTHREAD_H
#define SHADER_TOY_ANDROID_RENDERTHREAD_H

#include <mutex>
#include <thread>
#include "Renderer.h"

class RenderThread {
public:
    RenderThread();
    void stop();
    void start();
    void pause();
    void setFrameRate(int frameRate);
    int getFrameRate();
    int getRealFrameRate();

private:
    void run();
    std::shared_ptr<std::thread> mThread;
    std::mutex mMutex;
    std::atomic<bool> mStarted = false;
    Renderer* mRenderer;
};

#endif // SHADER_TOY_ANDROID_RENDERTHREAD_H