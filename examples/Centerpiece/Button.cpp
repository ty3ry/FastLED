#include <Button.h>


Button::Button(/* args */)
{
    io_key_state = IO_KEY_STATE_IDLE;
    tmr_key_scan.TimeOutSet(IO_KEY_SCAN_TIME);

    /** init digital pin */
    pinMode(BTN_PIN, INPUT_PULLUP);
}

Button::~Button()
{
}

/**
 * @brief read key button channel (digital read only)
 * @note
 */
uint8_t Button::ChannelKeyGet(void)
{
    uint8_t key_index = IO_CHANNEL_EMPTY;

    /** read pin, set as active low */
    if (!digitalRead(BTN_PIN)) {
        key_index = 0;
    
    } else if (0 /* reserved */) {

    } else {

    }

    return key_index;
}

/***
 * @brief   IO Button key scanning
 * @note
 */
IOKeyMsg Button::KeyScan(void)
{
    static uint8_t pre_key_index = IO_CHANNEL_EMPTY;
    uint8_t key_index;
    IOKeyMsg Msg = {IO_CHANNEL_EMPTY,   IO_KEY_UNKOWN_TYPE};

    if (!tmr_key_scan.IsTimeOut()) {
        return Msg;
    }
    tmr_key_scan.TimeOutSet(IO_KEY_SCAN_TIME);

    key_index = ChannelKeyGet();

    switch(io_key_state) {
        case IO_KEY_STATE_IDLE:
            if (key_index == IO_CHANNEL_EMPTY) return Msg;
            pre_key_index = key_index;
            tmr_key_wait.TimeOutSet(IO_KEY_JITTER_TIME);
            io_key_state = IO_KEY_STATE_JITTER;

        case IO_KEY_STATE_JITTER:
            if (pre_key_index != key_index) {
                pre_key_index = IO_CHANNEL_EMPTY;
                io_key_state = IO_KEY_STATE_IDLE;
            }
            else if (tmr_key_wait.IsTimeOut()) {
                tmr_key_wait.TimeOutSet(IO_KEY_CP_TIME);
                io_key_state = IO_KEY_STATE_PRESS_DOWN;
                Msg.index = pre_key_index;
                Msg.type = IO_KEY_PRESSED;
            }
            break;
        case IO_KEY_STATE_PRESS_DOWN:
            if (pre_key_index != key_index) {
                io_key_state = IO_KEY_STATE_IDLE;
                Msg.index = pre_key_index;
                Msg.type = IO_KEY_RELEASED;
            }
            else if (tmr_key_wait.IsTimeOut()) {
                tmr_key_wait.TimeOutSet(IO_KEY_CPH_TIME);
                io_key_state = IO_KEY_STATE_CP;
                Msg.index = pre_key_index;
                Msg.type = IO_KEY_LONG_PRESSED;
            }
            break;

        case IO_KEY_STATE_CP:
            if (pre_key_index != key_index) {
                io_key_state = IO_KEY_STATE_IDLE;
                Msg.index = pre_key_index;
                Msg.type = IO_KEY_LONG_RELEASED;
                pre_key_index = IO_CHANNEL_EMPTY;
            }
            else if (tmr_key_wait.IsTimeOut()) {
                tmr_key_wait.TimeOutSet(IO_KEY_CPH_TIME);
                Msg.index = pre_key_index;
                Msg.type = IO_KEY_LONG_PRESS_HOLD;
            }
            break;
        
        default: 
            io_key_state = IO_KEY_STATE_IDLE;
            pre_key_index = IO_CHANNEL_EMPTY;
            break;
    }

    return Msg;
}