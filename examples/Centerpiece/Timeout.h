#include <Arduino.h>

class Timeout
{
private:
    unsigned long int timeoutVal;
    unsigned long int tickvalCache;
    bool isTimeout;

public:
    Timeout(/* args */);
    ~Timeout();

    void TimeOutSet(unsigned long int timeout);
    bool IsTimeOut(void);
};


