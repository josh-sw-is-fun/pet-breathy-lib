#pragma once

#include "pet_breathy/point.hpp"
#include "pet_breathy/point_group_status.hpp"

namespace pb {

class PointGroup {
public:
    PointGroup(const size_t groupId, const size_t offsetIdx, const Points& points);

    size_t GetGroupId() const { return m_groupId; }
    size_t GetPointOffsetIdx() const { return m_offsetIdx; }
    Points GetPoints() const { return m_points; }
    PointGroupStatus GetStatus() const { return m_status; }
    
    void UpdatePoints(const Points& points);
    void Reset(const Points& points);
    

private:
    size_t m_groupId{0};
    size_t m_offsetIdx{0};
    Points m_points{};
    PointGroupStatus m_status;
};

}
