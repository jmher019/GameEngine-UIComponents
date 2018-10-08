#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>

using namespace std;

class Timer {
private:
    chrono::steady_clock::time_point start;
    chrono::steady_clock::time_point current;

public:
    Timer(void);

    Timer(const Timer& timer);

    Timer(Timer&& timer);

    Timer& operator=(const Timer& timer) noexcept;

    Timer& operator=(Timer&& timer) noexcept;

    chrono::duration<float> getTimeElapsed(void) noexcept;

    chrono::duration<float> getTotalTimeElapsed(void) noexcept;
};

#endif // !TIMER_HPP
