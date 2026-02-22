#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <future>
#include <CommonAPI/CommonAPI.hpp>
#include <v1/bmw/ivi/IVIProxy.hpp>

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[93m"
#define BLUE    "\033[94m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define RESET   "\033[0m"

using namespace v1::bmw::ivi;

void onSpeedLimit(const int16_t &speed)
{
    std::cout << CYAN
              << "[Client] Speed limit sign: "
              << speed << " km/h"
              << RESET << std::endl;
}

void onStopSign()
{
    std::cout << MAGENTA
              << "[Client] Stop sign detected"
              << RESET << std::endl;
}

void onRoadBlock()
{
    std::cout << YELLOW
              << "[Client] Road block detected"
              << RESET << std::endl;
}

void onCpuLoad(const float &usage)
{
    std::cout << BLUE
              << "[Client] CPU load broadcast: "
              << usage << "%"
              << RESET << std::endl;
}

void onCpuLoadRequested(const CommonAPI::CallStatus &status, const float &usage)
{
    if (status == CommonAPI::CallStatus::SUCCESS)
    {
        std::cout << GREEN
                  << "[Client] CPU load method call succeeded: "
                  << usage << "%"
                  << RESET << std::endl;
    }
    else
    {
        std::cout << RED
                  << "[Client] CPU load method call failed with status: "
                  << static_cast<int>(status)
                  << RESET << std::endl;
    }
}

int main()
{
    std::cout << "Starting IVI Client..." << std::endl;

    auto runtime = CommonAPI::Runtime::get();
    if (!runtime)
    {
        std::cerr << "Failed to get CommonAPI runtime!" << std::endl;
        return 1;
    }

    std::string domain = "local";
    std::string instance = "bmw.ivi.IVI";

    auto proxy = runtime->buildProxy<IVIProxy>(domain, instance);
    if (!proxy)
    {
        std::cerr << "Failed to build IVI proxy!" << std::endl;
        return 1;
    }

    std::cout << "Waiting for service..." << std::endl;
    while (!proxy->isAvailable())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    std::cout << "Service is available." << std::endl;

    // subscribe to events
    proxy->getNotifySpeedLimitSignDetectedEvent().subscribe(onSpeedLimit);
    proxy->getNotifyStopSignDetectedEvent().subscribe(onStopSign);
    proxy->getNotifyRoadBlockDetectedEvent().subscribe(onRoadBlock);
    proxy->getNotifyIVICpuLoadEvent().subscribe(onCpuLoad);

    std::cout << "Subscribed to events." << std::endl;

    std::thread requestThread([proxy]() {
        while (true)
        {
            std::this_thread::sleep_for(std::chrono::seconds(5));

            std::cout << GREEN
                      << "[Client] Requesting CPU load..."
                      << RESET << std::endl;

            auto future = proxy->requestIVICpuLoadAsync(onCpuLoadRequested);
            auto status = future.get();

            std::cout << GREEN
                      << "Future returned status: "
                      << static_cast<int>(status)
                      << RESET << std::endl;
        
            // CommonAPI::CallStatus callStatus;
            // float usage = 0.0f;
            // proxy->requestIVICpuLoad(callStatus, usage);            
        }
    });

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    requestThread.join();
    return 0;
}
