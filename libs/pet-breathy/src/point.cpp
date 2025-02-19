#include "pet_breathy/point.hpp"

#include <cmath>

namespace pb {

float CalcDistance(const Point &p0, const Point &p1) {
    const float dx = p1.x - p0.x;
    const float dy = p1.y - p0.y;
    return std::sqrtf(dx * dx + dy * dy);
}

} // namespace pb
