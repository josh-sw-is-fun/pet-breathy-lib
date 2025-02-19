#pragma once

#include "pet_breathy/point.hpp"

#include <vector>

namespace pb {

class PointMonitor {
  public:
    PointMonitor(const size_t frameWidth, const size_t frameHeight, const size_t maxPoints);

    void CheckForBadPoints(const Points &prevPoints, const Points &currPoints);

    const std::vector<size_t>& GetOutOfFrameIdxs() const { return m_outOfFrameIdxs; }
    const std::vector<size_t>& GetJumpIdxs() const { return m_jumpIdxs; }

    bool HasBadPoints() const { return !m_outOfFrameIdxs.empty() || !m_jumpIdxs.empty(); }

  private:
    size_t m_frameWidth{0};
    size_t m_frameHeight{0};

    std::vector<size_t> m_outOfFrameIdxs;
    std::vector<size_t> m_jumpIdxs;
};

} // namespace pb
