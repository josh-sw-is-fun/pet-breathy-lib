#include "pet_breathy/point_group.hpp"

#include <stdexcept>
#include <string.h>

namespace pb {

PointGroup::PointGroup(const size_t groupId, const size_t offsetIdx, const Points& points) : m_groupId(groupId), m_offsetIdx(offsetIdx), m_points(points) { }

void PointGroup::UpdatePoints(const Points& points) {
    if (points.size != m_points.size) {
        throw std::invalid_argument("Cannot update points, sizes do not match");
    }
    
    memcpy(m_points.p, points.p, points.size * sizeof(Point));
}

void PointGroup::Reset(const Points& points) {
    m_status.Reset();
    UpdatePoints(points);
}

}
