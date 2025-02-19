#pragma once

#include <cstddef>

namespace pb {

struct VideoInfo {
    float fps;
    size_t width;
    size_t height;
    size_t frameCount;
};

} // namespace pb
