#ifndef IVISERVICEIMPL_HPP_
#define IVISERVICEIMPL_HPP_

#include <v1/bmw/ivi/IVIStubDefault.hpp>
#include <thread>
#include <atomic>
#include <random>

class IVIServiceImpl : public v1::bmw::ivi::IVIStubDefault
{
public:
    IVIServiceImpl();
    virtual ~IVIServiceImpl();

    virtual void requestIVICpuLoad(const std::shared_ptr<CommonAPI::ClientId> _client,
                                   requestIVICpuLoadReply_t _reply) override;

    void startSimulation();
    void stopSimulation();

private:
    void simulationLoop();

    std::thread simulationThread_;
    std::atomic<bool> running_{false};
    std::default_random_engine generator_;
    std::uniform_int_distribution<int> signDist_;   // 0,1,2 for sign types
    std::uniform_real_distribution<float> cpuDist_; // 0.0 – 100.0
};

#endif // IVISERVICEIMPL_HPP_
