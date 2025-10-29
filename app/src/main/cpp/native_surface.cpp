#include "ALog.h"
#include "RenderThread.h"
#include "Renderer.h"
#include <android/native_window_jni.h>
#include <jni.h>
#include <memory>
#include <pthread.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "NativeSurfaceJNI"

class NativeSurfaceView {
public:
    NativeSurfaceView(RenderType type, ANativeWindow *window) : mType(type) {
        mRenderer = Renderer::create(mType, window);
        mThread.setRenderer(mRenderer);
    }
    void start() { mThread.start(); }
    void stop() { mThread.stop(); }
    void pause() { mThread.pause(); }
    void resume() { mThread.resume(); }

public:
    // Static JNI functions
    static jlong onSurfaceCreated(JNIEnv *env, jobject thiz, jobject surface) {
        ALOGD("%s", __func__);
        auto window = ANativeWindow_fromSurface(env, surface);
        auto renderer = new NativeSurfaceView(RenderType::GLES, window);
        return (jlong)renderer;
    }

    static void onSurfaceChanged(JNIEnv *env, jobject thiz, jlong nativePtr,
                                 jobject surface, jint format, jint width,
                                 jint height) {
        ALOGD("%s", __func__);
        if (nativePtr && surface) {
            auto nativeView = (NativeSurfaceView *)(nativePtr);
            auto window = ANativeWindow_fromSurface(env, surface);
            nativeView->mRenderer->update(window, width, height);
        }
    }

    static void onSurfaceDestroyed(JNIEnv *env, jobject thiz, jlong nativePtr,
                                   jobject surface) {
        ALOGD("%s", __func__);
        auto nativeSurfaceView = (NativeSurfaceView *)(nativePtr);
        delete nativeSurfaceView;
    }

    static void onSurfaceRedrawNeeded(JNIEnv *env, jobject thiz, jlong renderer,
                                      jobject surface) {
        ALOGD("%s", __func__);
    }

private:
    RenderType mType;
    RenderThread mThread;
    std::shared_ptr<Renderer> mRenderer;
};

#define JNI_CLASS_NAME "com/hweex/shadertoyandroid/ui/view/NativeSurfaceView"

// JNI method table
static JNINativeMethod gMethods[] = {
    {"onSurfaceCreatedNative", "(Landroid/view/Surface;)J",
     reinterpret_cast<void *>(NativeSurfaceView::onSurfaceCreated)},
    {"onSurfaceChangedNative", "(JLandroid/view/Surface;III)V",
     reinterpret_cast<void *>(NativeSurfaceView::onSurfaceChanged)},
    {"onSurfaceDestroyedNative", "(JLandroid/view/Surface;)V",
     reinterpret_cast<void *>(NativeSurfaceView::onSurfaceDestroyed)},
    {"onSurfaceRedrawNeededNative", "(JLandroid/view/Surface;)V",
     reinterpret_cast<void *>(NativeSurfaceView::onSurfaceRedrawNeeded)}};

// JNI registration function
extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) !=
        JNI_OK) {
        return JNI_ERR;
    }

    jclass clazz = env->FindClass(JNI_CLASS_NAME);
    if (clazz == nullptr) {
        return JNI_ERR;
    }

    if (env->RegisterNatives(clazz, gMethods,
                             sizeof(gMethods) / sizeof(gMethods[0])) < 0) {
        return JNI_ERR;
    }

    ALOGD("%s", "JNI registration successful");
    return JNI_VERSION_1_6;
}
