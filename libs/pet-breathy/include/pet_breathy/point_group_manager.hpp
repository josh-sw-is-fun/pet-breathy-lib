#pragma once

#include "pet_breathy/point_group.hpp"

#include <stdexcept>
#include <vector>

namespace pb {

class PointGroupManager {
  public:
    PointGroupManager(const int maxPoints);
    
    // References are kept to m_points, do not copy it
    PointGroupManager(PointGroupManager&) = delete;
    PointGroupManager& operator=(const PointGroupManager&) = delete;
    
    size_t GetCapacity() const { return m_maxPoints; }
    
    size_t GetSize() const { return m_pointIdx; }
    
    PointGroup& GetPointGroup(const size_t groupId) { return m_groups[groupId]; }
    
    PointGroup CreatePointGroup(const size_t numPoints);
  
  private:
    size_t m_maxPoints{0};
    size_t m_groupIdGen{0};
    
    std::vector<size_t> m_pointIdxToGroupIdLookup;
    
    std::vector<Point> m_points;
    std::vector<Point> m_scratchPoints;
    std::vector<PointGroup> m_groups;
    std::vector<size_t> m_badGroups;
};

} // namespace pb
