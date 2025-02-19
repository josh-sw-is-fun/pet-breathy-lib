#pragma once

#include <opencv2/core/types.hpp>

namespace pb {

using Point = cv::Point2f;

struct Points {
    Point *p;
    size_t size;
};

float CalcDistance(const Point &p0, const Point &p1);

} // namespace pb
