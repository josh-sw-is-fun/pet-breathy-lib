#pragma once

#include "pet_breathy/optical_flow.hpp"
#include "pet_breathy/patch_analyzer.hpp"
#include "pet_breathy/point_generator.hpp"
#include "pet_breathy/point_group_manager.hpp"
#include "pet_breathy/point_monitor.hpp"
#include "pet_breathy/signal_analyzer.hpp"
#include "pet_breathy/stats.hpp"
#include "pet_breathy/video_info.hpp"

#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>

namespace pb {

class PetBreathy {
  public:
    PetBreathy(const VideoInfo &info, const size_t maxPoints, const size_t decimation, cv::Mat &frame);

    void ProcessFrame(cv::Mat &frame);

    float GetRuntimePerFrame() const { return m_decimation / m_info.fps; };

    cv::Mat &GetProcessedFrame() { return m_frame; };

  private:
    void ProcessFrame();

    void PreprocessFrame(cv::Mat &frame);

    void Setup();

  private:
    VideoInfo m_info;
    size_t m_decimation{0};
    size_t m_frameCount{0};

    cv::Mat m_frame;

    PointGroupManager m_manager;
    PointMonitor m_monitor;
    OpticalFlow m_flow;
    SignalAnalyzer m_signalAnalyzer;
    PatchAnalyzer m_patchAnalyzer;
    PointGenerator m_pointGenerator;
    Stats m_stats;

    size_t m_maxStaticPoints;
    size_t m_maxPatchPoints;

    /*
        # Key:      patch id
        # Value:    A patch like object
        self.patch_lookup = { }
        self.static_patches = [ ]
        self.movable_patches = [ ]
        self.sorted_movable_patches = [ ]

        self.patch_id_gen = 0
     */
};

} // namespace pb
