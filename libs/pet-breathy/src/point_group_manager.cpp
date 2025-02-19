#include "pet_breathy/point_group_manager.hpp"

namespace pb {

PointGroupManager::PointGroupManager(const int maxPoints) : m_maxPoints(maxPoints) {
    m_groups.reserve(maxPoints);
    m_pointIdxToGroupIdLookup.reserve(maxPoints);

    m_points.reserve(maxPoints);
    m_scratchPoints.reserve(maxPoints);
}

PointGroup PointGroupManager::CreatePointGroup(const size_t numPoints) {
    if (m_groupIdGen >= m_groups.size()) {
        throw std::invalid_argument("Cannot create point group, invalid group ID");
    }
    
    if (numPoints + m_points.size() > m_points.capacity()) {
        throw std::invalid_argument("Cannot create point group, not enough points available");
    }
    
    const size_t groupId = m_groupIdGen++;
    const size_t pointIdx = m_points.size();
    
    m_points.resize(m_points.size() + numPoints);
    m_scratchPoints.resize(m_scratchPoints.size() + numPoints);
    
    for (size_t i = 0; i < numPoints; i++) {
        m_pointIdxToGroupIdLookup.push_back(groupId);
    }
    
    Points points;
    points.p = &m_points[pointIdx];
    points.size = numPoints;
    
    m_groups.emplace_back(PointGroup(groupId, m_points.size(), points));
    return m_groups.back();
}

void PointGroupManager::UpdatePoints(PointMonitor& monitor) {
    m_badGroups.clear();

    if (monitor.HasBadPoints()) {
        const auto& outOfFrameIdxs = monitor.GetOutOfFrameIdxs();
        
        for (auto idx : outOfFrameIdxs) {
            const auto groupId = m_pointIdxToGroupIdLookup[idx];
            m_groups[groupId].GetStatus().PointWentOutOfFrame();
            m_badGroups.insert(groupId);
        }
        
        const auto& jumpIdxs = monitor.GetJumpIdxs();
        
        for (auto idx : jumpIdxs) {
            const auto groupId = m_pointIdxToGroupIdLookup[idx];
            m_groups[groupId].GetStatus().PointJumped();
            m_badGroups.insert(groupId);
        }
    }
}

} // namespace pb
