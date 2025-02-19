#include "CLI/CLI.hpp"
#include <csignal>
#include <iostream>
#include <string>

#include "pet_breathy_runner.hpp"

struct CliArgs {
    std::string inVideoPath;
};

void Run(const CliArgs &args);
void HandleSignal(int signum);

PetBreathyRunner *g_runner{nullptr};

int main(int argc, char *argv[]) {
    CliArgs args;

    CLI::App app{"Demo pet-breathy-lib"};

    app.add_option("-i, --in-video-path", args.inVideoPath, "Input video file path");

    CLI11_PARSE(app, argc, argv);

    int exit_code = EXIT_SUCCESS;

    try {
        Run(args);
    } catch (const std::runtime_error &err) {
        std::cout << "Error occurred, what: " << err.what() << "\n";
        exit_code = EXIT_FAILURE;
    }

    return exit_code;
}

void Run(const CliArgs &args) {
    std::cout << "Running pet-breathy-lib demo with args:\n"
              << "- In video path: " << args.inVideoPath << "\n";

    auto runner = CreateRunnerFromVideoFile(args.inVideoPath);
    g_runner = &runner;

    if (std::signal(SIGINT, HandleSignal) == SIG_ERR) {
        throw std::runtime_error("Failed to register SIGINT handler");
    }
    if (std::signal(SIGQUIT, HandleSignal) == SIG_ERR) {
        throw std::runtime_error("Failed to register SIGQUIT handler");
    }

    runner.Run();
}

void HandleSignal(int) { g_runner->Stop(); }
