#pragma once

#include "pet_breathy/point.hpp"

#include <opencv2/core/mat.hpp>

namespace pb {

class OpticalFlow {
  public:
    OpticalFlow();
    void Calculate(cv::Mat &prevFrame, cv::Mat &nextFrame, std::vector<Point> &prevPoints,
                   std::vector<Point> &nextPoints);

  private:
    std::vector<unsigned char> m_status;
    std::vector<float> m_errors;

    cv::Size m_winSize;
};

} // namespace pb
