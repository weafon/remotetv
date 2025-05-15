#ifndef SAMSUNG_REMOTE_H
#define SAMSUNG_REMOTE_H

#include <Arduino.h>
extern const int kTransmissionDelay;   // Delay duration after IR command
#define IR_SEND_PIN PD2
enum RemoteCommand {
    RC_OFF = 0,
    RC_ON,
    RC_SOURCE,
    RC_MENU,
    RC_ENTER,
    RC_RETURN,
    RC_EXIT,
    RC_UP,
    RC_LEFT,
    RC_DOWN,
    RC_RIGHT,
	RC_PLAY,
	RC_PAUSE,
    RC_INVALID,
    RC_INVALID_KEY
};

void sendIRCommand(RemoteCommand command);
void IRinit();
#endif