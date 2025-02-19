#include "pet_breathy/pet_breathy.hpp"

namespace pb {

PetBreathy::PetBreathy(const VideoInfo &info, const size_t maxPoints, const size_t decimation, cv::Mat &frame)
    : m_info(info),
    m_decimation(decimation),
    m_manager(maxPoints),
    m_monitor(m_info.width, m_info.height, maxPoints),
    m_signalAnalyzer(m_info.fps, m_decimation),
    m_patchAnalyzer(m_signalAnalyzer),
    m_pointGenerator(m_info.width, m_info.height),
    m_stats(m_info, m_decimation) {
    (void)maxPoints;
    (void)frame;
}

void PetBreathy::ProcessFrame(cv::Mat &frame) {
    if ((m_frameCount % m_decimation) == 0) {
        PreprocessFrame(frame);
        ProcessFrame();
    }

    m_frameCount++;
}

void PetBreathy::ProcessFrame() {
    /*
     */
    
    /*
        frame = self._preprocess_frame(frame)

        old_points = self.manager.get_points()
        new_points, _, _ = \
            self.flow.calc(self.prev_frame, frame, old_points)

        self.prev_frame = frame

        self.monitor.check_for_bad_points(old_points, new_points)

        self.manager.update_points(new_points, self.monitor)

        # For each patch, add points to segments
        for patch in self.patch_lookup.values():
            patch.points_updated()

        if self.manager.has_bad_group_ids():
            self._handle_bad_point_groups()

        self._analyze_patches()

        if self.debug:
            self.debug_thing.set_frame(frame)
            self.debug_thing.patch_lookup = self.patch_lookup

            self.debug_thing.do_stuff(self.frame_count)
     */
}

void PetBreathy::PreprocessFrame(cv::Mat &frame) { cv::cvtColor(frame, m_frame, cv::COLOR_BGR2GRAY); }

void PetBreathy::Setup() {
    /*
        self.monitor = PointMonitor(self.info.width, self.info.height)
        self.manager = PointGroupManager(self.max_points)
        self.flow = OpticalFlow()
        self.signal_analyzer = SignalAnalyzer(self.info.fps, self.decimation)
        self.patch_analyzer = PatchAnalyzer(self.signal_analyzer)
        self.point_gen = self._create_point_gen()
        self.stats = Stats(self.info, self.decimation)

        self.frame_count = 0

        self.max_static_points = 4
        self.max_patch_points = self.max_points - self.max_static_points

        # Key:      patch id
        # Value:    A patch like object
        self.patch_lookup = { }
        self.static_patches = [ ]
        self.movable_patches = [ ]
        self.sorted_movable_patches = [ ]

        self.patch_id_gen = 0

        self._setup_static_points()

        if self.manager.get_size() != self.max_static_points:
            raise Exception('Added more static points then expected, fix it')

        self._setup_movable_patches()

        if self.debug:
            print('Static patch count:  %s' % len(self.static_patches))
            print('Movable patch count: %s' % len(self.movable_patches))

        if self.debug:
            self._setup_debug()
     */
}

} // namespace pb
