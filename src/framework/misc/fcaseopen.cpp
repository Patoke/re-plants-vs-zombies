#include "fcaseopen.h"
#include <algorithm>
#include <bits/fs_fwd.h>
#include <cctype>
#include <cstdlib>
#include <locale>
#include <memory>
#include <string>
#include <unordered_map>

#if !defined(_WIN32)
#include <stdlib.h>
#include <string.h>

#include <dirent.h>
#include <errno.h>
#include <unistd.h>

#include <iostream>
#include <filesystem>

constexpr inline std::vector<std::string> GetTokensFromPath(const std::string &path) {
    size_t pos = 0;
    size_t start = 0;
    std::vector<std::string> tokens;

    while ((pos = path.find('/', start)) != std::string::npos) {
        if (pos != start)
            tokens.push_back(path.substr(start, pos - start));
        start = pos + 1;
    }
    tokens.push_back(path.substr(start, path.length() - start));

    return tokens;
}

typedef std::vector<std::string> pathVec;
typedef std::unordered_map<std::string, std::unique_ptr<pathVec>> pathMap;
pathMap directoryCache;

constexpr bool compareCaseInsensitive(const std::string &a, const std::string &b) {
    return strcasecmp(a.c_str(), b.c_str()) < 0;
}

std::unique_ptr<pathVec> createCachedDirectory(const std::string &dir) {
    std::unique_ptr<pathVec> ret = std::make_unique<pathVec>();

    for (const auto &entry: std::filesystem::directory_iterator(dir)) {
        auto s = entry.path().string();
        ret->emplace_back(s.substr(s.rfind('/', std::string::npos) + 1));
    }

    std::sort(ret->begin(), ret->end(), compareCaseInsensitive);

    return ret;
}

constexpr inline std::string to_lowercase(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c){ return std::tolower(c); });
    return s;
}

std::string casepath(const std::string &path)
{
    // returns either a path that has upper/lower case fixed and points to an / existing file or returns empty string

    // split original path
    std::vector<std::string> tokens = GetTokensFromPath(path);

    // iterate through directories and check if we find case-insensitive matches to the path
    // NOTE: the first match will be taken
    std::string result;
    if (path[0] == '/')
        result = "/";
    else
        result = "./";

    for (size_t i = 0; i < tokens.size(); ++i) {
        std::string token = tokens[i];
        std::string current = result + token;

        if (!std::filesystem::exists(current)) {
            auto key = to_lowercase(result);

            pathVec *directory;
            if (auto search = directoryCache.find(key); search != directoryCache.end()) {
                directory = search->second.get();
            } else {
                directory = directoryCache.emplace(
                pathMap::value_type(
                    key, createCachedDirectory(result))
                ).first->second.get();
            }

            auto entry = std::lower_bound(
                directory->begin(), directory->end(),
                token, compareCaseInsensitive
            );

            if (entry != directory->end() && !compareCaseInsensitive(token, *entry)) {
                current = result + *entry;
            } else {
                return "";
            }
        }

        result = current + ((i != tokens.size()-1) ? "/" : "");
    }

    if (!std::filesystem::exists(result))
        return "";

    return result;
}
#endif

FILE* fcaseopen(char const* path, char const* mode)
{
    FILE* f = fopen(path, mode);
#if !defined(_WIN32)
    if (!f)
    {
        std::string r = casepath(path);
        if (r == "") return nullptr;
        f = fopen(r.c_str(), mode);
    }
#endif
    return f;
}
