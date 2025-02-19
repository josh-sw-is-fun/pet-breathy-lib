#pragma once

#include <chrono>
#include <string>

#include "pet_breathy/video_display.hpp"
#include "pet_breathy/video_reader.hpp"

enum class ExitStatus { JustExit, PauseAndExit, DoNothing };

class PetBreathyRunner {
  public:
    PetBreathyRunner(pb::VideoReader reader);

    void Run();
    void Stop();

  private:
    void UpdatePointBudget();

    ExitStatus WaitKeyPress(const float frameElapsed);
    ExitStatus WaitKeyPressWithTimeout(const int delayMillis);

  private:
    pb::VideoReader m_reader;
    pb::VideoInfo m_info;
    pb::VideoDisplay m_display;

    size_t m_fps{0};
    size_t m_decimation{0};
    size_t m_pointBudget{0};

    bool m_readyToRun{false};
    std::chrono::steady_clock::time_point m_startTime;
};

PetBreathyRunner CreateRunnerFromVideoFile(const std::string &videoPath);
