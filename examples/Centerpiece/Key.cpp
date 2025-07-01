#include "Key.h"

static const unsigned int IOKEY_TAB[][5] =
{
    //KEY_PRESS       SHORT_RELEASE     LONG_PRESS     KEY_HOLD      LONG_PRESS_RELEASE
	{MSG_NONE,        MSG_MODE,      	MSG_COLOR_MODE_BLINK,       MSG_NONE,     MSG_NONE    },
    {MSG_NONE,        MSG_NONE,         MSG_NONE,       MSG_NONE,     MSG_NONE    },
};

Key::Key(/* args */)
{
}

Key::~Key()
{
}

MessageId Key::Scan(void)
{
    MessageId Msg = MSG_NONE;
    IOKeyMsg IoKeyMsg;

    IoKeyMsg = mButton.KeyScan();

    if (IoKeyMsg.index != IO_CHANNEL_EMPTY && IoKeyMsg.type != IO_KEY_UNKOWN_TYPE) {
        mFuncId = IOKEY_TAB[IoKeyMsg.index][IoKeyMsg.type - 1];
    }

    Msg = (MessageId) mFuncId;
    mFuncId = 0;

    return Msg;
}