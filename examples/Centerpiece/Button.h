#include "Timeout.h"

#define IO_CHANNEL_EMPTY                (0xFF)

#define IO_KEY_SCAN_TIME			20
#define IO_KEY_JITTER_TIME			30
#define IO_KEY_CP_TIME				1000 	//for hold or long press
#define IO_KEY_CPH_TIME			    200		//for long long press
#define	IO_KEY_FULL_VAL			    4096

/** pin number definition using arduino style */
#define BTN_PIN     13  //PB7

/** num untuk type dari IOKeyType */
enum IOKeyType {
    IO_KEY_UNKOWN_TYPE = 0,
	IO_KEY_PRESSED,
	IO_KEY_RELEASED,
	IO_KEY_LONG_PRESSED,
	IO_KEY_LONG_PRESS_HOLD,
	IO_KEY_LONG_RELEASED,
};

/** struct untuk message */
struct IOKeyMsg {
    unsigned int index;
    unsigned int type;
};

/** enum untuk io key state */
enum IO_KEY_STATE {
    IO_KEY_STATE_IDLE,
	IO_KEY_STATE_JITTER,
	IO_KEY_STATE_PRESS_DOWN,
	IO_KEY_STATE_CP
};

class Button
{
private:
    IO_KEY_STATE io_key_state;
    Timeout tmr_key_scan;
    Timeout tmr_key_wait;
    
public:
    Button(/* args */);
    ~Button();

    uint8_t ChannelKeyGet(void);
    IOKeyMsg KeyScan(void);
};
