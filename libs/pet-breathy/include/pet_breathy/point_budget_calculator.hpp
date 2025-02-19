#pragma once

#include <opencv2/core/mat.hpp>

#include <cstddef>

namespace pb {

size_t EstimateMaxPoints(const float frameSec, cv::Mat &prevFrame, cv::Mat &nextFrame);

}
