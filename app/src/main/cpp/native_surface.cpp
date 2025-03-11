#include <jni.h>
#include <android/native_window_jni.h>
#include "ALog.h"
#include "Renderer.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "NativeSurfaceJNI"
extern "C"
JNIEXPORT jlong JNICALL
Java_com_hweex_shadertoyandroid_ui_view_NativeSurfaceView_onSurfaceCreatedNative(JNIEnv *env,
                                                                                 jobject thiz,
                                                                                 jobject surface) {
    // TODO: implement onSurfaceCreatedNative()
    ALOGD("%s", __func__ );
    auto window = ANativeWindow_fromSurface(env, surface);
    auto renderer = Renderer::create(RenderType::GLES, window);
    // renderer->start();
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
    auto native_renderer = (GLRenderer*)(renderer);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_hweex_shadertoyandroid_ui_view_NativeSurfaceView_onSurfaceDestroyedNative(JNIEnv *env,
                                                                                   jobject thiz,
                                                                                   jlong renderer,
                                                                                   jobject surface) {
    // TODO: implement onSurfaceDestroyedNative()
    ALOGD("%s", __func__ );
    auto native_renderer = (GLRenderer*)(renderer);
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
    auto native_renderer = (Renderer*)(renderer);
    // native_renderer->start();
}
