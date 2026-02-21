#include "IVIServiceImpl.hpp"
#include <iostream>
#include <chrono>
#include <thread>

IVIServiceImpl::IVIServiceImpl()
    : generator_(std::random_device{}()),
      signDist_(0, 2),
      cpuDist_(0.0f, 100.0f)
{
}

IVIServiceImpl::~IVIServiceImpl()
{
    stopSimulation();
}

void IVIServiceImpl::requestIVICpuLoad(const std::shared_ptr<CommonAPI::ClientId> _client,
                                       requestIVICpuLoadReply_t _reply)
{
    std::cout << "[Service] requestIVICpuLoad called" << std::endl;
    float usage = cpuDist_(generator_);
    std::cout << "[Service] returning CPU load = " << usage << std::endl;
    _reply(usage);
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

void IVIServiceImpl::simulationLoop()
{
    while (running_)
    {
        // Simulate a sign detection every 5–10 seconds
        std::this_thread::sleep_for(std::chrono::seconds(5 + (rand() % 6)));
        if (!running_)
            break;

        int signType = signDist_(generator_);
        switch (signType)
        {
        case 0: // stop sign
            std::cout << "[Service] fire StopSignDetected" << std::endl;
            fireNotifyStopSignDetectedEvent();
            break;
        case 1: // speed limit sign
        {
            int16_t speed = 30 + (rand() % 70); // 30..99 km/h
            std::cout << "[Service] fire SpeedLimitSignDetected: " << speed << std::endl;
            fireNotifySpeedLimitSignDetectedEvent(speed);
        }
        break;
        case 2: // road block
            std::cout << "[Service] fire RoadBlockDetected" << std::endl;
            fireNotifyRoadBlockDetectedEvent();
            break;
        }

        // Additionally fire a CPU load event every 3 seconds
        std::this_thread::sleep_for(std::chrono::seconds(3));
        if (!running_)
            break;
        float cpu = cpuDist_(generator_);
        std::cout << "[Service] fire IVICpuLoad: " << cpu << std::endl;
        fireNotifyIVICpuLoadEvent(cpu);
    }
}
