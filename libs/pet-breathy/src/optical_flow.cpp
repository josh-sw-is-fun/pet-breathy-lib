#include "pet_breathy/optical_flow.hpp"

#include <opencv2/video/tracking.hpp>

namespace {

// - (3,3)       Points are on bath salts
// - (9,9)       Points go all over the place
// - (21,21)     Points little eratic
// - (51,51)     Seems ... to work alright
// - (101,101)   Points do not move enough
// - (201, 201)  Seem to be working alright for FFT analysis at least
static constexpr int WIN_SIZE = 71;

} // namespace

namespace pb {

OpticalFlow::OpticalFlow() : m_winSize(WIN_SIZE, WIN_SIZE) {}

void OpticalFlow::Calculate(cv::Mat &prevFrame, cv::Mat &nextFrame, std::vector<Point> &prevPoints,
                            std::vector<Point> &nextPoints) {

    cv::calcOpticalFlowPyrLK(prevFrame, nextFrame, prevPoints, nextPoints, m_status, m_errors, m_winSize);
}

} // namespace pb
