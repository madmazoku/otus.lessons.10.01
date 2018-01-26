#pragma once

#ifdef __unix__
#cmakedefine PROJECT_VERSION_PATCH @PROJECT_VERSION_PATCH@
#else
#define PROJECT_VERSION_PATCH 1
#endif

inline int build_version()
{
    return PROJECT_VERSION_PATCH;
}