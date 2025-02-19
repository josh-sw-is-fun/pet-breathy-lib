#pragma once

#include "pet_breathy/video_info.hpp"

namespace pb {

class Stats {
  public:
    Stats(const VideoInfo& info, const size_t decimation);
};

} // namespace pb
