#include "pet_breathy/video_reader.hpp"

namespace pb {

VideoReader::VideoReader(const std::string &fileName) : m_cap(fileName) {
    m_info.fps = static_cast<float>(m_cap.get(cv::CAP_PROP_FPS));
    m_info.width = static_cast<size_t>(m_cap.get(cv::CAP_PROP_FRAME_WIDTH));
    m_info.height = static_cast<size_t>(m_cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    m_info.frameCount = static_cast<size_t>(m_cap.get(cv::CAP_PROP_FRAME_COUNT));
}

const VideoInfo &VideoReader::GetInfo() const { return m_info; }

bool VideoReader::GetNextFrame(cv::Mat &frame) { return m_cap.read(frame); }

size_t VideoReader::GetFrameNum() const { return static_cast<size_t>(m_cap.get(cv::CAP_PROP_POS_FRAMES)); }

} // namespace pb
