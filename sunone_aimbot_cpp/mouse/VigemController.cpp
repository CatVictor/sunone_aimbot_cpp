#include "VigemController.h"
#include <iostream>

VigemController::VigemController()
    : client_(nullptr), target_(nullptr), connected_(false)
{
    client_ = vigem_alloc();
    if (!client_)
    {
        std::cerr << "[Vigem] Failed to allocate client" << std::endl;
        return;
    }
    if (vigem_connect(client_) != VIGEM_ERROR_NONE)
    {
        std::cerr << "[Vigem] Failed to connect" << std::endl;
        vigem_free(client_);
        client_ = nullptr;
        return;
    }
    target_ = vigem_target_x360_alloc();
    if (vigem_target_add(client_, target_) != VIGEM_ERROR_NONE)
    {
        std::cerr << "[Vigem] Failed to add target" << std::endl;
        vigem_disconnect(client_);
        vigem_free(client_);
        client_ = nullptr;
        target_ = nullptr;
        return;
    }
    connected_ = true;
}

VigemController::~VigemController()
{
    if (client_ && target_)
        vigem_target_remove(client_, target_);
    if (target_)
        vigem_target_free(target_);
    if (client_)
    {
        vigem_disconnect(client_);
        vigem_free(client_);
    }
}

void VigemController::move(int x, int y)
{
    std::lock_guard<std::mutex> lock(mtx_);
    if (!connected_) return;
    XUSB_REPORT report{};
    report.sThumbRX = static_cast<SHORT>(x);
    report.sThumbRY = static_cast<SHORT>(y);
    vigem_target_x360_update(client_, target_, report);
}

void VigemController::press(int button)
{
    std::lock_guard<std::mutex> lock(mtx_);
    if (!connected_) return;
    XUSB_REPORT report{};
    report.wButtons = static_cast<WORD>(button);
    vigem_target_x360_update(client_, target_, report);
}

void VigemController::release(int button)
{
    std::lock_guard<std::mutex> lock(mtx_);
    if (!connected_) return;
    XUSB_REPORT report{};
    vigem_target_x360_update(client_, target_, report);
}
