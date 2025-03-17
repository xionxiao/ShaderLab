#include <jni.h>
#include <android/native_window_jni.h>
#include <memory>
#include <pthread.h>
#include "ALog.h"
#include "RenderThread.h"
#include "Renderer.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "NativeSurfaceJNI"
class NativeSurfaceView {
public:
    NativeSurfaceView(RenderType type, ANativeWindow* window) : mType(type) {
        mRenderer = Renderer::create(mType, window);
        mThread.setRenderer(mRenderer);
    }
    void start() { mThread.start(); }
    void pause() { mThread.pause(); }
private:
    RenderType mType;
    RenderThread mThread;
    std::shared_ptr<Renderer> mRenderer;
};

extern "C"
JNIEXPORT jlong JNICALL
Java_com_hweex_shadertoyandroid_ui_view_NativeSurfaceView_onSurfaceCreatedNative(JNIEnv *env,
                                                                                 jobject thiz,
                                                                                 jobject surface) {
    // TODO: implement onSurfaceCreatedNative()
    ALOGD("%s", __func__ );
    auto window = ANativeWindow_fromSurface(env, surface);
    auto renderer = new NativeSurfaceView(RenderType::GLES, window);
    return (jlong) renderer;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_hweex_shadertoyandroid_ui_view_NativeSurfaceView_onSurfaceChangedNative(JNIEnv *env,
                                                                                 jobject thiz,
                                                                                 jlong renderer,
                                                                                 jobject surface,
                                                                                 jint format,
                                                                                 jint width,
                                                                                 jint height) {
    // TODO: implement onSurfaceChangedNative()
    ALOGD("%s", __func__ );
    auto native_view= (NativeSurfaceView*)(renderer);
    native_view->start();

}

extern "C"
JNIEXPORT void JNICALL
Java_com_hweex_shadertoyandroid_ui_view_NativeSurfaceView_onSurfaceDestroyedNative(JNIEnv *env,
                                                                                   jobject thiz,
                                                                                   jlong renderer,
                                                                                   jobject surface) {
    // TODO: implement onSurfaceDestroyedNative()
    ALOGD("%s", __func__ );
    auto native_renderer = (NativeSurfaceView*)(renderer);
    delete native_renderer;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_hweex_shadertoyandroid_ui_view_NativeSurfaceView_onSurfaceRedrawNeededNative(JNIEnv *env,
                                                                                      jobject thiz,
                                                                                      jlong renderer,
                                                                                      jobject surface) {
    // TODO: implement onSurfaceRedrawNeededNative()
    ALOGD("%s", __func__ );
    // auto native_renderer = (NativeSurfaceView*)(renderer);
    // native_renderer->start();
}
