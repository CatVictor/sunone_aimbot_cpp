#ifndef VIGEM_CONTROLLER_H
#define VIGEM_CONTROLLER_H

#include <ViGEm/Client.h>
#include <mutex>

class VigemController
{
public:
    VigemController();
    ~VigemController();

    bool isOpen() const { return connected_; }

    void move(int x, int y);
    void press(int button);
    void release(int button);

private:
    PVIGEM_CLIENT client_;
    PVIGEM_TARGET target_;
    bool connected_;
    std::mutex mtx_;
};

#endif // VIGEM_CONTROLLER_H
