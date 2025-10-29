#ifndef SHADER_TOY_ANDROID_RENDERER_H
#define SHADER_TOY_ANDROID_RENDERER_H

#include <android/looper.h>
#include <android/native_activity.h>
#include <EGL/egl.h>
#include <android/native_window.h>
#include "Shader.h"

enum class RenderType {
    GLES,
    VULKAN,
    SKIA_GL,
    SKIA_VK,
};

class Renderer {
public:
    static std::shared_ptr<Renderer> create(RenderType type, ANativeWindow* window);
    virtual ~Renderer() = 0;
    virtual void render();
    virtual void update(ANativeWindow *window, int width, int height);

protected:
    explicit Renderer(ANativeWindow* window);

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
    void update(ANativeWindow *window, int width, int height) override;
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
