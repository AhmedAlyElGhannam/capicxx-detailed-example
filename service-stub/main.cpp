#include <iostream>
#include <memory>
#include <thread>
#include <CommonAPI/CommonAPI.hpp>
#include "IVIServiceImpl.hpp"

int main()
{
    std::cout << "Starting IVI Service..." << std::endl;

    auto runtime = CommonAPI::Runtime::get();
    if (!runtime)
    {
        std::cerr << "Failed to get CommonAPI runtime!" << std::endl;
        return 1;
    }

    std::string domain = "local";
    std::string instance = "bmw.ivi.IVI";

    auto service = std::make_shared<IVIServiceImpl>();
    bool registered = runtime->registerService(domain, instance, service);
    if (!registered)
    {
        std::cerr << "Failed to register IVI service!" << std::endl;
        return 1;
    }

    std::cout << "Service registered successfully." << std::endl;

    while (true)
        std::this_thread::sleep_for(std::chrono::seconds(1));

    return 0;
}
