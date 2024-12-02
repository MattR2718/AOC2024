#pragma once

#include <iostream>
#include <chrono>
#include <unordered_map>
#include <string>
#include <sstream>

#include "fmt/core.h"
#include "fmt/color.h"

namespace aoc_utils {

    struct timer_config {
        int id;
        std::string units = "milliseconds";
        std::string label = "";
        std::string description = "";
    };

    class Timer {
    public:
        void create_timer(const timer_config& config) {
            TimerData data;
            data.label = config.label;
            data.description = config.description;
            data.unit_name = config.units;
            timers[config.id] = data;
        }

        void begin(int id) {
            if (timers.find(id) != timers.end()) {
                timers[id].start_time = std::chrono::high_resolution_clock::now();
            }
            else {
                std::cerr << "Error: Timer with id " << id << " does not exist.\n";
            }
        }

        void end(int id) {
            if (timers.find(id) != timers.end()) {
                auto end_time = std::chrono::high_resolution_clock::now();
                timers[id].end_time = end_time;
                timers[id].is_stopped = true;
            }
            else {
                std::cerr << "Error: Timer with id " << id << " does not exist.\n";
            }
        }

        void display_timer(int id) const {
            if (timers.find(id) != timers.end()) {
                const TimerData& data = timers.at(id);
                if (!data.is_stopped) {
                    std::cerr << "Error: Timer with id " << id << " has not been stopped yet.\n";
                    return;
                }

                fmt::print(fmt::fg(fmt::color::blue), "============================== Timer Details ==============================\n");
                fmt::print(fmt::fg(fmt::color::green), "Timer ID      : ");
                fmt::print("{}\n", id);
                fmt::print(fmt::fg(fmt::color::green), "Label         : ");
                fmt::print("{}\n", data.label);
                fmt::print(fmt::fg(fmt::color::green), "Description   : ");
                fmt::print("{}\n", data.description);
                fmt::print(fmt::fg(fmt::color::green), "Elapsed Time  : ");
                fmt::print("{} {}\n", calculate_elapsed_time(data), data.unit_name);
                fmt::print(fmt::fg(fmt::color::blue), "===========================================================================\n");
            }
            else {
                std::cerr << "Error: Timer with id " << id << " does not exist.\n";
            }
        }

        void display_all() const {
            for (const auto& [id, data] : timers) {
                if (data.is_stopped) {
                    display_timer(id);
                }
                else {
                    std::cout << "Timer ID: " << id << " is still running.\n";
                }
            }
        }

    private:
        struct TimerData {
            std::chrono::time_point<std::chrono::steady_clock> start_time;
            std::chrono::time_point<std::chrono::steady_clock> end_time;
            bool is_stopped = false;
            std::string label;
            std::string description;
            std::string unit_name;
        };

        std::unordered_map<int, TimerData> timers;

        static double calculate_elapsed_time(const TimerData& data) {
            auto duration = data.end_time - data.start_time;
            if (data.unit_name == "milliseconds") {
                return std::chrono::duration<double, std::milli>(duration).count();
            }
            else if (data.unit_name == "microseconds") {
                return std::chrono::duration<double, std::micro>(duration).count();
            }
            else if (data.unit_name == "nanoseconds") {
                return std::chrono::duration<double, std::nano>(duration).count();
            }
            else if (data.unit_name == "seconds") {
                return std::chrono::duration<double>(duration).count();
            }
            else {
                return std::chrono::duration<double>(duration).count(); // Default to seconds
            }
        }
    };

}
