#include "pet_breathy/point_budget_calculator.hpp"
#include "pet_breathy/optical_flow.hpp"

#include <chrono>

namespace {

float MeasureRuntime(const size_t pointCount, cv::Mat &prevFrame, cv::Mat &nextFrame);
std::vector<cv::Point2f> CreatePoints(const size_t pointCount, const size_t frameHeight,
                                      const size_t frameWidth);

} // namespace

namespace pb {

/**
 * Calculates a very rough estimate the number of points that can be used to track features.
 * @param frameSec Time between frames
 * @return Number of points that can be used to track features
 */
size_t EstimateMaxPoints(const float frameSec, cv::Mat &prevFrame, cv::Mat &nextFrame) {
    /* Calculates the runtime to run an optical flow calculation with A points then the same thing with B
     * points. Based on the runtime of both calculations, linear interpolate to find the number of points or
     * point budget that can be used to find signals.
     *
     * For example, if we wish to take .2 seconds per frame after decimation, then we could calculate the
     * number of points that can be used while keeping the runtime around 75% (for example) of .2 seconds.
     *
     * This is just for a rough estimate of run time not exact. I would rather measure the number of points
     * I can use then guess at 300 or something like that.
     *
     * The current and previous frames are fed into the optical flow object. The runtime of this operation is
     * measured.
     */
    const size_t aPointCount = 100;
    const size_t bPointCount = 1000;

    const float aRuntime = MeasureRuntime(aPointCount, prevFrame, nextFrame);
    const float bRuntime = MeasureRuntime(bPointCount, prevFrame, nextFrame);

    const float x0 = static_cast<float>(aPointCount);
    const float y0 = aRuntime;

    const float x1 = bPointCount;
    const float y1 = bRuntime;

    const float m = (y1 - y0) / (x1 - x0);

    // y = mx + b -> b = y - mx
    const float b = y0 - m * x0;

    // y = mx + b -> x = (y - b) / m
    const size_t maxPointEst = static_cast<size_t>(std::round((frameSec - b) / m));

    return maxPointEst;
}

} // namespace pb

namespace {

float MeasureRuntime(const size_t pointCount, cv::Mat &prevFrame, cv::Mat &nextFrame) {
    pb::OpticalFlow flow;

    auto points =
        CreatePoints(pointCount, static_cast<size_t>(prevFrame.rows), static_cast<size_t>(prevFrame.cols));
    std::vector<cv::Point2f> nextPoints;

    using std::chrono::high_resolution_clock;
    const auto t0 = high_resolution_clock::now();
    flow.Calculate(prevFrame, nextFrame, points, nextPoints);
    const auto t1 = high_resolution_clock::now();

    const float runtime = std::chrono::duration<float>(t1 - t0).count();

    return runtime;
}

std::vector<cv::Point2f> CreatePoints(const size_t pointCount, const size_t frameHeight,
                                      const size_t frameWidth) {
    std::vector<cv::Point2f> points;

    for (size_t i = 0; i < pointCount; i++) {
        const float x = static_cast<float>(i % frameWidth);
        const float y = static_cast<float>((i / static_cast<float>(frameHeight)) * frameWidth);
        points.emplace_back(cv::Point2f(x, y));
    }

    return points;
}

} // namespace
