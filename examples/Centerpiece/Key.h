#include "Message.h"
#include "Button.h"

class Key
{
private:
    unsigned int mFuncId;
    Button mButton;

public:
    Key(/* args */);
    ~Key();

    MessageId Scan(void);
};


