#include "pet_breathy/video_display.hpp"

#include <opencv2/highgui.hpp>

namespace pb {

VideoDisplay::VideoDisplay(const std::string &name, const size_t width, const size_t height, const float ratio)
    : m_name(name) {
    cv::namedWindow(m_name, cv::WINDOW_NORMAL);

    cv::resizeWindow(m_name, static_cast<int>(width * ratio), static_cast<int>(height * ratio));
}

void VideoDisplay::Show(cv::Mat &frame) const { cv::imshow(m_name, frame); }

void VideoDisplay::Move(const int x, const int y) const { cv::moveWindow(m_name, x, y); }

void VideoDisplay::Close() const { cv::destroyWindow(m_name); }

bool VideoDisplay::IsOpen() const { return cv::getWindowProperty(m_name, cv::WND_PROP_VISIBLE) > 0; }

} // namespace pb
