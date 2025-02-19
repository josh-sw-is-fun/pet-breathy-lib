#pragma once

#include "pet_breathy/video_info.hpp"

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

namespace pb {

class VideoReader {
  public:
    VideoReader(const std::string &fileName);

    const VideoInfo &GetInfo() const;
    bool GetNextFrame(cv::Mat &frame);
    size_t GetFrameNum() const;

  private:
    cv::VideoCapture m_cap;
    VideoInfo m_info;
};

} // namespace pb
