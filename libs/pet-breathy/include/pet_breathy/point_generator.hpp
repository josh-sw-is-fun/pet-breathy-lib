#pragma once

#include "pet_breathy/point.hpp"

#include <random>

namespace pb {

class PointGenerator {
  public:
    PointGenerator(const size_t width, const size_t height);
    
    PointVector Generate(const size_t numPoints);
    Point Generate();

  private:
    size_t m_minWidth{0};
    size_t m_minHeight{0};
    size_t m_maxWidth{0};
    size_t m_maxHeight{0};
    
    std::random_device m_dev;
    std::mt19937 m_rng;
    std::uniform_int_distribution<std::mt19937::result_type> m_widthDist;
    std::uniform_int_distribution<std::mt19937::result_type> m_heightDist;
};

} // namespace pb
