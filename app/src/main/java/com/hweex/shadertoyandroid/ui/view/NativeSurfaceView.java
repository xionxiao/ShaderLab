package com.hweex.shadertoyandroid.ui.view;

import android.content.Context;
import android.util.AttributeSet;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import androidx.annotation.NonNull;

public class NativeSurfaceView extends SurfaceView implements SurfaceHolder.Callback2 {
    static String LOG_TAG = "NativeSurfaceView";
    static {
        System.loadLibrary("native-surface");
    }

    public enum RenderType {
        GLES,
        VULKAN,
        SKIA_GL,
        SKIA_VK
    };

    /*
     * Change Renderer
     */
    public void setRendererType(RenderType type) {
    }

    public NativeSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
        Log.d(LOG_TAG, "Constructor");
        getHolder().addCallback(this);
    }

    @Override
    public void surfaceCreated(@NonNull SurfaceHolder surfaceHolder) {
        Surface surface = surfaceHolder.getSurface();
        Log.d(LOG_TAG, "SurfaceCreated: " + surface);
        if (surface != null) {
            nativeRenderer = onSurfaceCreatedNative(surface);
        }
    }

    @Override
    public void surfaceChanged(@NonNull SurfaceHolder surfaceHolder, int format, int width, int height) {
        Surface surface = surfaceHolder.getSurface();
        Log.d(LOG_TAG, "SurfaceChanged: " + surface);
        if (surface != null) {
            onSurfaceChangedNative(nativeRenderer, surface, format, width, height);
        }
    }

    @Override
    public void surfaceDestroyed(@NonNull SurfaceHolder surfaceHolder) {
        Surface surface = surfaceHolder.getSurface();
        Log.d(LOG_TAG, "SurfaceDestroyed: " + surface);
        if (surface != null) {
            onSurfaceDestroyedNative(nativeRenderer, surface);
        }
    }

    @Override
    public void surfaceRedrawNeeded(@NonNull SurfaceHolder surfaceHolder) {
        Surface surface = surfaceHolder.getSurface();
        Log.d(LOG_TAG, "SurfaceRedrawNeeded: " + surface);
        if (surface != null) {
            onSurfaceRedrawNeededNative(nativeRenderer, surface);
        }
    }

    private long nativeRenderer = 0L;
    private native long onSurfaceCreatedNative(Surface surface);
    private native void onSurfaceChangedNative(long renderer, Surface surface, int format, int width, int height);
    private native void onSurfaceDestroyedNative(long renderer, Surface surface);
    private native void onSurfaceRedrawNeededNative(long renderer, Surface surface);
}
