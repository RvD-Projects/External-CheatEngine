#include <chrono>

class Timer
{
public:
    Timer(const long &msTime = 0)
    {
        if (msTime < 0)
            throw std::out_of_range("msTime");

        this->duration = std::chrono::milliseconds(msTime);
    }

    void Start(const long &msTimeOverride = NULL)
    {
        if (msTimeOverride > 0)
            this->duration = std::chrono::milliseconds(msTimeOverride);

        startTime = std::chrono::steady_clock::now();
        endTime = startTime + duration;
        isRunning = true;
    }

    void Stop()
    {
        endTime = std::chrono::steady_clock::now();
        isRunning = false;
    }

    void Reset(const bool &stopTimer = true)
    {
        if (stopTimer)
            Stop();

        startTime = std::chrono::steady_clock::now();
        endTime = startTime + duration;
    }

    bool IsRunning() const
    {
        return isRunning;
    }

    std::chrono::duration<double> Duration() const
    {
        return duration;
    }

    std::chrono::duration<double> Elapsed() const
    {
        if (isRunning)
        {
            return std::chrono::steady_clock::now() - startTime;
        }
        else
        {
            return endTime - startTime;
        }
    }

    std::chrono::duration<double> Remaining() const
    {
        if (isRunning)
        {
            auto now = std::chrono::steady_clock::now();
            if (now >= endTime)
            {
                return std::chrono::duration<double>(0);
            }
            return endTime - now;
        }
        else
        {
            return endTime - startTime;
        }
    }

    std::string ToString() const
    {
        auto timeLeft = std::chrono::duration_cast<std::chrono::milliseconds>(Remaining()).count();
        auto seconds = timeLeft / 1000;
        auto milliseconds = timeLeft % 1000;

        std::ostringstream oss;
        oss << seconds << "." << std::setw(3) << std::setfill('0') << milliseconds;
        return oss.str();
    }

private:
    bool isRunning;
    std::chrono::milliseconds duration;
    std::chrono::time_point<std::chrono::steady_clock> startTime;
    std::chrono::time_point<std::chrono::steady_clock> endTime;
};