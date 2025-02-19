#include "pet_breathy_runner.hpp"

#include "pet_breathy/pet_breathy.hpp"
#include "pet_breathy/point_budget_calculator.hpp"
#include "pet_breathy/video_reader.hpp"

#include <opencv2/highgui.hpp>

// TODO DELETE
#include <chrono>
#include <iostream>

PetBreathyRunner::PetBreathyRunner(pb::VideoReader reader)
    : m_reader(reader), m_info(m_reader.GetInfo()), m_display("main", m_info.width, m_info.height, 0.5f) {
    m_fps = static_cast<size_t>(std::round(m_info.fps));
    switch (m_fps) {
    case 30:
        m_decimation = 5;
        break;
    case 60:
        m_decimation = 10;
        break;
    default:
        throw std::runtime_error("Frame rate not supported: " + std::to_string(m_info.fps));
    };

    UpdatePointBudget();

    m_readyToRun = true;
}

void PetBreathyRunner::Run() {
    const auto &info = m_reader.GetInfo();
    std::cout << "Info:\n"
              << "- FPS:         " << std::round(info.fps) << "\n"
              << "- Width:       " << info.width << "\n"
              << "- Height:      " << info.height << "\n"
              << "- Frame count: " << info.frameCount << "\n"
              << "- Max points:  " << m_pointBudget << "\n";

    cv::Mat frame;
    if (!m_reader.GetNextFrame(frame)) {
        throw std::runtime_error("Could not read frame to setup");
    }

    pb::PetBreathy breathy(info, m_pointBudget, m_decimation, frame);

    const float maxFrameRuntime = breathy.GetRuntimePerFrame();
    m_startTime = std::chrono::steady_clock::now();

    bool pauseBeforeExiting = false;

    while (m_readyToRun) {
        const auto t0 = std::chrono::steady_clock::now();

        if (m_reader.GetNextFrame(frame)) {
            breathy.ProcessFrame(frame);

            m_display.Show(breathy.GetProcessedFrame());
        } else {
            m_readyToRun = false;
        }

        const auto t1 = std::chrono::steady_clock::now();
        const auto frameRuntime = std::chrono::duration<float>(t1 - t0).count();

        if (frameRuntime > maxFrameRuntime) {
            std::cout << "Took too long! " << frameRuntime << " sec\n";
        }

        const auto exitStatus = WaitKeyPress(frameRuntime);

        if (exitStatus != ExitStatus::DoNothing) {
            m_readyToRun = false;

            if (exitStatus == ExitStatus::PauseAndExit) {
                pauseBeforeExiting = true;
            }
        }
    }

    if (pauseBeforeExiting) {
        while (WaitKeyPressWithTimeout(10) == ExitStatus::DoNothing) {
        }
    }
}

void PetBreathyRunner::Stop() { m_readyToRun = false; }

void PetBreathyRunner::UpdatePointBudget() {
    cv::Mat f0, f1;
    if (!m_reader.GetNextFrame(f0) || !m_reader.GetNextFrame(f1)) {
        throw std::runtime_error("Could not read first 2 frames");
    }

    m_pointBudget = pb::EstimateMaxPoints(m_decimation / static_cast<double>(m_fps), f0, f1);
}

ExitStatus PetBreathyRunner::WaitKeyPress(const float frameElapsed) {
    const float frameRate = 1.0f / m_info.fps;

    const float actualElapsedTime =
        std::chrono::duration<float>(std::chrono::steady_clock::now() - m_startTime).count();
    const float expectedElapsedTime = frameRate * m_reader.GetFrameNum();
    const float diffTime = expectedElapsedTime - actualElapsedTime;

    const int waitTimeMillis = std::max(1, static_cast<int>(diffTime * 1000));

    return WaitKeyPressWithTimeout(waitTimeMillis);
}

ExitStatus PetBreathyRunner::WaitKeyPressWithTimeout(const int delayMillis) {
    const int key = cv::waitKey(delayMillis);
    switch (key) {
    case 'c':
        return ExitStatus::JustExit;
    case 's':
        return ExitStatus::PauseAndExit;
    }
    return ExitStatus::DoNothing;
}

PetBreathyRunner CreateRunnerFromVideoFile(const std::string &videoPath) {
    return PetBreathyRunner(std::move(pb::VideoReader(videoPath)));
}
