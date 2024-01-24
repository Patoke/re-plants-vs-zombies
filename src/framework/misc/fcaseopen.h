#ifndef fcaseopen_h
#define fcaseopen_h

#include <stdio.h>
#include <string>
#include <vector>

FILE* fcaseopen(char const* path, char const* mode);

#if !defined(_WIN32)
std::string casepath(const std::string &path);
#else
#define casepath(path) (path)
#endif

#endif
