#include <Timer.hpp>

Timer::Timer(void) {
    start = chrono::steady_clock::now();
    current = start;
}

Timer::Timer(const Timer& timer):
    start(timer.start),
    current(timer.current) {
}

Timer::Timer(Timer&& timer):
    start(move(timer.start)),
    current(move(timer.current)) {
}

Timer& Timer::operator=(const Timer& timer) noexcept {
    start = timer.start;
    current = timer.current;

    return *this;
}

Timer& Timer::operator=(Timer&& timer) noexcept {
    start = move(timer.start);
    current = move(timer.current);

    return *this;
}

chrono::duration<float> Timer::getTimeElapsed(void) noexcept {
    chrono::steady_clock::time_point next = chrono::steady_clock::now();
    chrono::duration<float> timeElapsed = next - current;
    current = next;

    return timeElapsed;
}

chrono::duration<float> Timer::getTotalTimeElapsed(void) noexcept {
    current = chrono::steady_clock::now();
    return current - start;
}