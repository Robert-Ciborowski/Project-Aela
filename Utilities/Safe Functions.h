/*
 * File: Safe Functions
 * Author: Robert Ciborowski
 * Date: 09/09/2018
 * Description: A file which will perform the requested operation using the best "safe"
 *              function the compiler will provide.
 */

#pragma once
#include <cstdio>
#include <cassert>
#include <cerrno>
#include <functional>

static int fopen_safe(FILE **f, const char *name, const char *mode) {
    int ret = 0;
    assert(f);
    
#if defined __MINGW32__ || defined __APPLE__
    *f = fopen(name, mode);
#endif
    
#if defined (_MSC_VER)
    fopen_s(f, name, mode);
#endif
    
    if (!*f)
        ret = errno;
    return ret;
}

static int wfopen_safe(FILE** f, const wchar_t* name, const wchar_t* mode) {
    int ret = 0;
    assert(f);

#if defined __MINGW32__ || defined __APPLE__
    * f = fopen(name, mode);
#endif

#if defined (_MSC_VER)
    _wfopen_s(f, name, mode);
#endif

    if (!*f)
        ret = errno;
    return ret;
}

template<typename... Args> static int sscanf_safe(const char* c, const char* format, Args... args) {
#if defined __MINGW32__ || defined __APPLE__
    return sscanf(c, format, args...);
#endif
    
#if defined (_MSC_VER)
    return sscanf_s(c, format, args...);
#endif
    
    return 0;
}
