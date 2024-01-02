// include the needed header files
#include "KLS_Timer.h"
#include "KLS_String.h"

// unclutter the global namespace
namespace KLS
{
    // class constructor
    KLS_Timer::KLS_Timer()
    {
    }

    // class destructor
    KLS_Timer::~KLS_Timer()
    {
    }

    // pause the timer
    void KLS_Timer::pause()
    {
        m_Paused = true;
    }

    // reumse the timer
    void KLS_Timer::resume()
    {
        m_Paused = false;
        m_CurrentTime = std::chrono::high_resolution_clock::now();
        m_PreviousTime = m_CurrentTime;
        m_DeltaTime = 0;
        //m_TotalTime = 0;
        m_FPSTime = 0;
        m_NumberOfFrames = 0;
    }

    // frame emthod
    double KLS_Timer::tick()
    {
        if (m_Paused)
        {
            m_DeltaTime = 0;
            return 0;
        }

        // calculate elapsed time since the last frame
        m_CurrentTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        auto duration = currentTime.time_since_epoch();
        m_Time = duration.count() * std::chrono::high_resolution_clock::period::num / static_cast<double>(std::chrono::high_resolution_clock::period::den);

        std::chrono::duration<double> timeDifference = std::chrono::duration_cast<std::chrono::duration<double>>(m_CurrentTime - m_PreviousTime);
        m_DeltaTime = timeDifference.count();

        // update previous time
        m_PreviousTime = m_CurrentTime;

        // increment the numebr of frames
        m_NumberOfFrames++;

        // increment the total amount of time
        m_TotalTime += m_DeltaTime;

        m_FPSTime += m_DeltaTime;

        int MaxFrames = 100;

        // if we have rendered more than the max amount
        if (m_NumberOfFrames > MaxFrames)
        {
            // calculate the FPS (we do this every maxframes to prevent wild swings)
            m_FPS = m_NumberOfFrames / m_FPSTime;
            m_NumberOfFrames -= MaxFrames;
            m_FPSTime = 0;
        }

        return m_DeltaTime;
    }

    // current framerate
    double KLS_Timer::getFrameRate() const
    {
        return m_FPS;
    }

    // helper methods
    std::string KLS_Timer::getTotalTimeString()
    {
        return toString(m_TotalTime);
    }

    std::string KLS_Timer::getDeltaTimeString()
    {
        return toString(m_DeltaTime);
    }

    std::string KLS_Timer::getFrameRateString()
    {
        return toString(m_FPS);
    }

} // end namespace