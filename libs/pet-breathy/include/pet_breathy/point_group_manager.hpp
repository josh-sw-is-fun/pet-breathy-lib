#pragma once

#include "pet_breathy/point_group.hpp"
#include "pet_breathy/point_monitor.hpp"

#include <stdexcept>
#include <vector>
#include <set>

namespace pb {

class PointGroupManager {
  public:
    PointGroupManager(const int maxPoints);
    
    // References are kept to m_points, do not copy it
    PointGroupManager(PointGroupManager&) = delete;
    PointGroupManager& operator=(const PointGroupManager&) = delete;
    
    size_t GetPointCapacity() const { return m_maxPoints; }
    
    size_t GetPointCount() const { return m_points.size(); }
    
    PointGroup& GetPointGroup(const size_t groupId) { return m_groups[groupId]; }
    
    PointGroup CreatePointGroup(const size_t numPoints);
    
    void UpdatePoints(PointMonitor& monitor);
    
    PointVector& GetPoints() { return m_points; }
    PointVector& GetScratchPoints() { return m_scratchPoints; }
    
    const std::set<size_t>& GetBadGroupIds() const { return m_badGroups; }
    bool HasBadGroupIds() const { return !m_badGroups.empty(); }
  
  private:
    size_t m_maxPoints{0};
    size_t m_groupIdGen{0};
    
    std::vector<size_t> m_pointIdxToGroupIdLookup;
    
    PointVector m_points;
    PointVector m_scratchPoints;
    std::vector<PointGroup> m_groups;
    std::set<size_t> m_badGroups;
};

} // namespace pb
