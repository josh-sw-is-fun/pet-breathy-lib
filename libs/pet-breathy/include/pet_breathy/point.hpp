#pragma once

#include <vector>

#include <opencv2/core/types.hpp>

namespace pb {

using Point = cv::Point2f;
using PointVector = std::vector<Point>;

struct Points {
    Point *p;
    size_t size;
};

float CalcDistance(const Point &p0, const Point &p1);

} // namespace pb
