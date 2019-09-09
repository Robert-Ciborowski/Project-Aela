#pragma once

#include "Event.h"
#include <functional>

namespace Aela {
    typedef std::function<void(Event*)> EventListener;
}

#if defined __MINGW32__ || defined __APPLE__
#define bindListener(f, v) ({                   \
auto fn = &f;                               \
std::bind(fn, v, std::placeholders::_1);    \
})
#endif

#if defined (_MSC_VER)
#define bindListener(f, v) std::bind(&f, v, std::placeholders::_1)
#endif
