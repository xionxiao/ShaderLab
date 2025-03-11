#ifndef SHADER_TOY_ANDROID_RENDERER_H
#define SHADER_TOY_ANDROID_RENDERER_H

#include <thread>
#include <mutex>
#include <atomic>

#include <android/looper.h>
#include <android/native_activity.h>
#include <EGL/egl.h>
#include "Shader.h"

class Renderer {
public:
    explicit Renderer(ANativeWindow* window);
    virtual ~Renderer() = 0;
    virtual void render();

protected:
    ANativeWindow* mWindow;
    // TODO: use condition variable
    std::shared_ptr<Shader> mShader;
};

class GLRenderer : public Renderer {
public:
    explicit GLRenderer(ANativeWindow *window);
    ~GLRenderer() override;
    void render() override;
    void drawFrame();
    void submit();
private:
    void initGLES();
    void releaseSurfaceContext();
    void createSurfaceContext();

    EGLDisplay mDisplay;
    EGLSurface mSurface;
    EGLContext mContext;
};

class SkiaRenderer : public Renderer {

};

class VkRenderer : public Renderer {

};

#endif //SHADER_TOY_ANDROID_RENDERER_H
