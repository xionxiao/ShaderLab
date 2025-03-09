#include <chrono>
#include <GLES3/gl3.h>
#include "include/ALog.h"
#include "include/Renderer.h"
#include "include/Shader.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "ShaderToyRenderer"

using namespace std::chrono_literals;
#define CORNFLOWER_BLUE 100 / 255.f, 149 / 255.f, 237 / 255.f, 1
#define CORNFLOWER_RED 200 / 255.f, 149 / 255.f, 237 / 255.f, 1

Renderer::Renderer(ANativeWindow *window) : mWindow(window) {
}

Renderer::~Renderer() {
    // TODO: stop thread
    this->stop();
}

void Renderer::render() {
    while (true) {
       std::this_thread::sleep_for(1000ms);
       std::lock_guard<std::mutex> lock(this->mMutex);
       this->drawFrame();
       this->submit();
    }
}

void Renderer::stop() {
    mStarted.store(false, std::memory_order_relaxed);
}

void Renderer::start() {
    mThread = std::make_shared<std::thread>(&Renderer::render, this);
    mStarted.store(true, std::memory_order_relaxed);
}

void Renderer::drawFrame() {
    ALOGD("%s", "Renderer::draw()");
    if (mShader) {
        mShader->execute();
    }
}

void Renderer::submit() {

}

void Renderer::pause() {

}

GLRenderer::GLRenderer(ANativeWindow* window) :
        Renderer(window),
        mDisplay(EGL_NO_DISPLAY),
        mSurface(EGL_NO_SURFACE),
        mContext(EGL_NO_CONTEXT) {
    initGLES();
}

void GLRenderer::initGLES() {
    EGLint majorVersion;
    EGLint minorVersion;
    mDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(mDisplay, &majorVersion, &minorVersion);
    createSurfaceContext();
}

GLRenderer::~GLRenderer() {
    releaseSurfaceContext();
    if (mDisplay != EGL_NO_DISPLAY) {
        eglTerminate(mDisplay);
        mDisplay = EGL_NO_DISPLAY;
    }
}

void GLRenderer::createSurfaceContext() {
    if (mDisplay != EGL_NO_DISPLAY) {
        constexpr EGLint attrs[] = {
                EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
                EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                EGL_BLUE_SIZE, 8,
                EGL_GREEN_SIZE, 8,
                EGL_RED_SIZE, 8,
                EGL_ALPHA_SIZE, 8,
                EGL_DEPTH_SIZE, 24,
                EGL_NONE
        };

        EGLint numConfigs;
        eglChooseConfig(mDisplay, attrs, nullptr, 0, &numConfigs);

        auto configs = new EGLConfig[numConfigs];
        eglChooseConfig(mDisplay, attrs, configs, numConfigs, &numConfigs);

        ALOGD("numConfigs %d", numConfigs);

        mSurface = eglCreateWindowSurface(mDisplay, configs[0], mWindow, nullptr);
        EGLint contextAttrs[] = {
                EGL_CONTEXT_CLIENT_VERSION, 3,
                EGL_NONE
        };
        mContext = eglCreateContext(mDisplay, configs[0], nullptr, nullptr);
    }
}

void GLRenderer::releaseSurfaceContext() {
    if (mDisplay != EGL_NO_DISPLAY) {
        this->stop();
        eglMakeCurrent(mDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (mContext != EGL_NO_CONTEXT) {
            eglDestroyContext(mDisplay, mContext);
            mContext = EGL_NO_CONTEXT;
        }
        if (mSurface != EGL_NO_SURFACE) {
            eglDestroySurface(mDisplay, mSurface);
            mSurface = EGL_NO_SURFACE;
        }
    }
}

void GLRenderer::drawFrame() {
    EGLint width;
    eglQuerySurface(mDisplay, mSurface, EGL_WIDTH, &width);
    EGLint height;
    eglQuerySurface(mDisplay, mSurface, EGL_HEIGHT, &height);
    ALOGD("%s %d %d", "GLRenderer::drawFrame", width, height);

    eglMakeCurrent(mDisplay, mSurface, mSurface, mContext);

    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(CORNFLOWER_BLUE);
}

void GLRenderer::submit() {
    eglSwapBuffers(mDisplay, mSurface);
}

