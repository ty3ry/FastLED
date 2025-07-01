#include <Timeout.h>

#define GET_TICK()  millis()

Timeout::Timeout(/* args */)
{
}

void Timeout::TimeOutSet(unsigned long int timeout)
{
    timeoutVal = timeout;
    tickvalCache = GET_TICK();
}

bool Timeout::IsTimeOut(void)
{
    if (GET_TICK() >= timeoutVal + tickvalCache) {
        isTimeout = 1;
        return 1;
    }
    isTimeout = 0;
    return 0;
}

Timeout::~Timeout()
{
}