#ifndef TIME_H
#define TIME_H

#include <chrono>

class Time
{
public:
    static double getTime() //returns current time from the moment of starting application
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_epoch).count() / 1000000000.0;
    }
protected:
private:
    static std::chrono::steady_clock::time_point m_epoch; //time of starting application
};

std::chrono::steady_clock::time_point Time::m_epoch = std::chrono::high_resolution_clock::now();

#endif // TIME_H