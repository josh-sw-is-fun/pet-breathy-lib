#include "pet_breathy/point_generator.hpp"

// Placement of the static points in the frame. The ratio R is a percentage of the width and height. If the
// height is 100 and width is 200 and ratio is 15%, the 4 static points are placed as follows:
// - Top left:      { 30,       15       }
// - Top right:     { 200 - 30, 15       }
// - Bottom left:   { 30,       100 - 15 }
// - Bottom right:  { 200 - 30, 100 - 15 }
static const float STATIC_POINT_PLACEMENT_RATIO = 0.15f;

namespace pb {

PointGenerator::PointGenerator(const size_t width, const size_t height) :
    m_minWidth(static_cast<size_t>(STATIC_POINT_PLACEMENT_RATIO * width)),
    m_minHeight(static_cast<size_t>(STATIC_POINT_PLACEMENT_RATIO * height)),
    m_maxWidth(width - m_minWidth),
    m_maxHeight(height - m_minHeight),
    m_rng(m_dev()),
    m_widthDist(m_minWidth, m_maxWidth),
    m_heightDist(m_minHeight, m_maxHeight) { }

std::vector<Point> PointGenerator::Generate(const size_t numPoints) {
    std::vector<Point> points(numPoints);
    for (size_t i = 0; i < numPoints; i++) {
        Point& p = points[i];
        p.x = m_widthDist(m_rng);
        p.y = m_heightDist(m_rng);
    }
    return points;
}

Point PointGenerator::Generate() {
    Point p;
    p.x = m_widthDist(m_rng);
    p.y = m_heightDist(m_rng);
    return p;
}

} // namespace pb
