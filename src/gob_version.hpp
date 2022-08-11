#ifndef GOBLIB_VERSION_HPP
#define GOBLIB_VERSION_HPP

#define GOBLIB_VERSION_MAJOR 0
#define GOBLIB_VERSION_MINOR 2
#define GOBLIB_VERSION_PATCH 0

#define GOBLIB_VERSION_STRINGIFY_AGAIN(x) #x
#define GOBLIB_VERSION_STRINGIFY(x) GOBLIB_VERSION_STRINGIFY_AGAIN(x)

#define GOBLIB_VERSION_VALUE ((GOBLIB_VERSION_MAJOR << 16) | (GOBLIB_VERSION_MINOR << 8) | (GOBLIB_VERSION_PATCH))
#define GOBLIB_VERSION_STRING GOBLIB_VERSION_STRINGIFY(GOBLIB_VERSION_MAJOR.GOBLIB_VERSION_MINOR.GOBLIB_VERSION_PATCH)

#endif
