#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <future>
#include <CommonAPI/CommonAPI.hpp>
#include <v1/bmw/ivi/IVIProxy.hpp>

using namespace v1::bmw::ivi;

void onSpeedLimit(const int16_t &speed)
{
    std::cout << "[Client] Speed limit sign: " << speed << " km/h" << std::endl;
}

void onStopSign()
{
    std::cout << "[Client] Stop sign detected" << std::endl;
}

void onRoadBlock()
{
    std::cout << "[Client] Road block detected" << std::endl;
}

void onCpuLoad(const float &usage)
{
    std::cout << "[Client] CPU load broadcast: " << usage << "%" << std::endl;
}

void onCpuLoadRequested(const CommonAPI::CallStatus &status, const float &usage)
{
    if (status == CommonAPI::CallStatus::SUCCESS)
    {
        std::cout << "[Client] CPU load method call succeeded: " << usage << "%" << std::endl;
    }
    else
    {
        std::cout << "[Client] CPU load method call failed with status: " 
                  << static_cast<int>(status) << std::endl;
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

    // wait until proxy is fully initialized before making calls
    // proxy->getCompletionFuture().wait();

    std::thread requestThread([proxy]() {
        while (true)
        {
            std::this_thread::sleep_for(std::chrono::seconds(5));
            // std::cout << "[Client] Requesting CPU load..." << std::endl;
            // auto future = proxy->requestIVICpuLoadAsync(onCpuLoadRequested);
            // auto status = future.get();
            // std::cout << "Future returned status: " << static_cast<int>(status) << std::endl;

            std::cout << "[Client] Requesting CPU load..." << std::endl;
        
            CommonAPI::CallStatus callStatus;
            float usage = 0.0f;
            proxy->requestIVICpuLoad(callStatus, usage);
            
            std::cout << "[Client] CallStatus raw value: " << static_cast<int>(callStatus) << std::endl;
            std::cout << "[Client] Usage: " << usage << std::endl;
        }
    });

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    requestThread.join();
    return 0;
}
