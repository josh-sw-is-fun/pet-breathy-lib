#pragma once

#include <opencv2/core/mat.hpp>

#include <string>

namespace pb {

class VideoDisplay {
  public:
    VideoDisplay(const std::string &name, const size_t width, const size_t height, const float ratio);

    void Show(cv::Mat &frame) const;
    void Move(const int x, const int y) const;
    void Close() const;
    bool IsOpen() const;

  private:
    std::string m_name;
};

} // namespace pb
