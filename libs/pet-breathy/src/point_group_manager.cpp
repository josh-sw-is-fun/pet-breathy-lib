#include "pet_breathy/point_group_manager.hpp"

namespace pb {

PointGroupManager::PointGroupManager(const int maxPoints) : m_maxPoints(maxPoints) {
    m_groups.reserve(maxPoints);
    m_pointIdxToGroupIdLookup.reserve(maxPoints);

    m_points.reserve(maxPoints);
    m_scratchPoints.reserve(maxPoints);
    
    // List of group ids that have bad points
    m_badGroups.resize(maxPoints);
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
    
    m_groups.push_back(groupId);
    
    for (size_t i = 0; i < numPoints; i++) {
        m_pointIdxToGroupIdLookup.push_back(groupId);
    }
    
    Points points;
    points.p = &m_points[pointIdx];
    points.size = numPoints;
    
    return PointGroup(groupId, m_points.size(), points);
}

/*
    def update_points(self, new_points: np.ndarray, point_monitor: PointMonitor):
        out_of_frame_gids = set()
        jump_gids = set()
        
        if point_monitor.has_bad_points():
            out_of_frame_idxs = point_monitor.get_out_of_frame_idxs()
            
            for idx in out_of_frame_idxs:
                group_id = self.point_idx_to_group_id_lookup[idx]
                self.groups[group_id].get_status().point_went_out_of_frame()
                out_of_frame_gids.add(int(group_id))
            
            jump_idxs = point_monitor.get_jump_idxs()
            
            for idx in jump_idxs:
                group_id = self.point_idx_to_group_id_lookup[idx]
                self.groups[group_id].get_status().point_jumped()
                jump_gids.add(int(group_id))
            
            if self.debug:
                print('out: %s, jump: %s' % (out_of_frame_gids, jump_gids))
        
        self.bad_group_ids = out_of_frame_gids | jump_gids
        self.points.copy(new_points)

    def get_bad_group_ids(self) -> list:
        return self.bad_group_ids

    def has_bad_group_ids(self) -> bool:
        return len(self.bad_group_ids) > 0
 */

} // namespace pb
