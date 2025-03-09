#ifndef SHADERTOYANDROID_ATRACE_H
#define SHADERTOYANDROID_ATRACE_H

#pragma onece

#include <android/trace.h>

#define ATRACE_NAME(name) ScopedTrace ___tracer(name)
#define ATRACE_CALL() ATRACE_NAME(__FUNCTION__)

class ScopedTrace {
public:
    inline ScopedTrace(const char *name) {
        ATrace_beginSection(name);
    }

    inline ~ScopedTrace() {
        ATrace_endSection();
    }
};

#endif //SHADERTOYANDROID_ATRACE_H
