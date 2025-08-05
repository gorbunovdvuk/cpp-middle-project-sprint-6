#pragma once

#include <cstdio>
#include <iomanip>
#include <mutex>
#include <sstream>
#include <string>

class Logger {
public:
    static Logger &Get() {
        static Logger instance;
        return instance;
    }

    void Log(const std::string &message) {
        auto now   = std::chrono::system_clock::now();
        auto us = duration_cast<std::chrono::microseconds>(now - std::chrono::floor<std::chrono::seconds>(now)).count();

        std::time_t tt = std::chrono::system_clock::to_time_t(now);

        std::stringstream stream;
        stream << std::put_time(std::gmtime(&tt), "%Y-%m-%d %H:%M:%S")
            << "." << std::setw(6) << std::setfill('0') << us
            << ": " << message;

        fprintf(stdout, "%s\n", stream.str().c_str());
    }

    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;
    ~Logger() = default;

private:
    Logger() {
        file_ = stdout;
        // отключаем буферизацию
        setvbuf(file_, nullptr, _IONBF, 0);
    }

    FILE *file_;
};
