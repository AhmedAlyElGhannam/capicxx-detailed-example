#include "IVIServiceImpl.hpp"
#include <iostream>
#include <chrono>
#include <thread>

IVIServiceImpl::IVIServiceImpl()
    : generator_(std::random_device{}()),
      signDist_(0, 2),
      cpuDist_(0.0f, 100.0f)
{
    startSimulation();
}

IVIServiceImpl::~IVIServiceImpl()
{
    stopSimulation();
}

void IVIServiceImpl::startSimulation()
{
    if (running_)
        return;
    running_ = true;
    simulationThread_ = std::thread(&IVIServiceImpl::simulationLoop, this);
}

void IVIServiceImpl::stopSimulation()
{
    if (running_)
    {
        running_ = false;
        if (simulationThread_.joinable())
            simulationThread_.join();
    }
}

void IVIServiceImpl::requestIVICpuLoad(
    const std::shared_ptr<CommonAPI::ClientId> _client,
    requestIVICpuLoadReply_t _reply)
{
    std::cout << GREEN
              << "[Service] requestIVICpuLoad called"
              << RESET << std::endl;

    float usage = cpuDist_(generator_);

    std::cout << GREEN
              << "[Service] returning CPU load = "
              << usage
              << RESET << std::endl;

    _reply(usage);
}

void IVIServiceImpl::simulationLoop()
{
    static int16_t speed = 0;
    static float cpu = 0.0f;

    while (running_)
    {
        // broadcast CPU load event
        cpu = cpuDist_(generator_);
        std::cout << BLUE
                  << "[Service] fire IVICpuLoad: "
                  << cpu
                  << RESET << std::endl;

        fireNotifyIVICpuLoadEvent(cpu);

        std::this_thread::sleep_for(std::chrono::seconds(1));

        int signType = signDist_(generator_);
        switch (signType)
        {
            case 0: // stop sign
                std::cout << MAGENTA
                          << "[Service] fire StopSignDetected"
                          << RESET << std::endl;

                fireNotifyStopSignDetectedEvent();
            break;

            case 1: // speed limit sign
                speed = 50 + (rand() % 70); // 50..120 km/h
                std::cout << CYAN
                          << "[Service] fire SpeedLimitSignDetected: "
                          << speed
                          << RESET << std::endl;

                fireNotifySpeedLimitSignDetectedEvent(speed);
            break;

            case 2: // road block
                std::cout << YELLOW
                          << "[Service] fire RoadBlockDetected"
                          << RESET << std::endl;

                fireNotifyRoadBlockDetectedEvent();
            break;
        }
    }
}
