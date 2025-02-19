#include "pet_breathy/point_monitor.hpp"

namespace pb {

PointMonitor::PointMonitor(const size_t frameWidth, const size_t frameHeight, const size_t maxPoints)
    : m_frameWidth(frameWidth), m_frameHeight(frameHeight) {

    m_outOfFrameIdxs.reserve(maxPoints);
    m_jumpIdxs.reserve(maxPoints);
}

void PointMonitor::CheckForBadPoints(const Points &prevPoints, const Points &currPoints) {
    // Too small of a jump, too many points are filtered
    // Too large of a jump messes with the frequency domain analysis (FFT of
    // time domain samples)
    static constexpr float MAX_JUMP_DIST = 50.0f;

    m_outOfFrameIdxs.resize(0);
    m_jumpIdxs.resize(0);

    for (size_t i = 0; i < currPoints.size; i++) {
        const auto &currPoint = currPoints.p[i];
        const auto &prevPoint = prevPoints.p[i];

        if (!(0 < currPoint.x && m_frameWidth <= currPoint.x) ||
            !(0 < currPoint.y && m_frameHeight <= currPoint.y)) {
            m_outOfFrameIdxs.push_back(i);
        }
        
        const auto dist = CalcDistance(prevPoint, currPoint);
        if (dist > MAX_JUMP_DIST) {
            m_jumpIdxs.push_back(i);
        }
    }
}

} // namespace pb
