#include <GLES3/gl3.h>
#include <android/native_window.h>
#include <cstddef>
#include <memory>
#include "ALog.h"
#include "Renderer.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "ShaderToyRenderer"

using namespace std::chrono_literals;
#define CORNFLOWER_BLUE 100 / 255.f, 149 / 255.f, 237 / 255.f, 1
#define CORNFLOWER_RED 200 / 255.f, 149 / 255.f, 237 / 255.f, 1

std::shared_ptr<Renderer> Renderer::create(RenderType type, ANativeWindow* window) {
    if (window == nullptr) {
        return nullptr;
    }
    switch (type) {
    case RenderType::GLES:
        return std::shared_ptr<Renderer>(new GLRenderer(window));
    case RenderType::VULKAN:
    case RenderType::SKIA_GL:
    case RenderType::SKIA_VK:
      break;
    }
    return nullptr;
}

Renderer::Renderer(ANativeWindow *window) : mWindow(window) {
}

Renderer::~Renderer() {
    // TODO: stop thread
}

void Renderer::render() {
    ALOGD("%s", "Renderer::draw()");
    if (mShader) {
        mShader->execute();
    }
}

void Renderer::update(ANativeWindow *window, int width, int height) {
    ALOGD("Renderer::update: window=%p, width=%d, height=%d", window, width, height);
    // 基类实现，子类应该重写这个函数
    mWindow = window;
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

void GLRenderer::update(ANativeWindow *window, int width, int height) {
    ALOGD("GLRenderer::update: window=%p, width=%d, height=%d", window, width, height);

    // 如果窗口发生变化，需要重新创建EGL表面
    if (window != mWindow) {
        ALOGD("Window changed, recreating EGL surface: old=%p, new=%p", mWindow, window);
        // 释放旧的EGL资源
        releaseSurfaceContext();
        // 更新窗口引用
        mWindow = window;
        // 重新创建EGL表面和上下文
        createSurfaceContext();
    }
    // 如果尺寸发生变化，更新视口
    if (width > 0 && height > 0) {
        EGLint currentWidth, currentHeight;
        eglQuerySurface(mDisplay, mSurface, EGL_WIDTH, &currentWidth);
        eglQuerySurface(mDisplay, mSurface, EGL_HEIGHT, &currentHeight);

        if (currentWidth != width || currentHeight != height) {
            ALOGD("Surface size changed: %dx%d -> %dx%d", currentWidth, currentHeight, width, height);
            // 确保EGL上下文是当前的
            eglMakeCurrent(mDisplay, mSurface, mSurface, mContext);
            // 更新视口
            glViewport(0, 0, width, height);
        }
    }
}

void GLRenderer::render() {
    drawFrame();
    submit();
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
