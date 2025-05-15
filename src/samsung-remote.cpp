#include <Arduino.h>

#include <IRremote.hpp> // include the library
#include "samsung-remote.h"  
const int kTransmissionDelay = 100;

void IRinit() 
{
	// Initialize the IR sender
	IrSender.begin(IR_SEND_PIN);
	Serial.println("IR Sender initialized.");
}


void sendIRCommand(RemoteCommand command) 
{
    // Map the command to the corresponding IR code
    // and send the command using the IR sender
    const char* message = nullptr;
    uint32_t irCode = 0;

    switch (command) {
        case RC_OFF:
            irCode = 0xE0E019E6; // off
            message = "transmitting command for off";
            break;
        case RC_ON:
            irCode = 0xE0E09966; // on
            message = "sent: on";
            break;
        case RC_SOURCE:
            irCode = 0xE0E0807F; // source
            message = "sent: source";
            break;
        case RC_MENU:
            irCode = 0xE0E058A7; // menu
            message = "sent: menu";
            break;
        case RC_ENTER:
            irCode = 0xE0E016E9; // enter
            message = "sent: enter";
            break;
        case RC_RETURN:
            irCode = 0xE0E01AE5; // return
            message = "sent: return";
            break;
        case RC_EXIT:
            irCode = 0xE0E0B44B; // exit
            message = "sent: exit";
            break;
        case RC_UP:
            irCode = 0xE0E006F9; // up
            message = "sent: up";
            break;
        case RC_LEFT:
            irCode = 0xE0E0A659; // left
            message = "sent: left";
            break;
        case RC_DOWN:
            irCode = 0xE0E08679; // down
            message = "sent: down";
            break;
        case RC_RIGHT:
            irCode = 0xE0E046B9; // right
            message = "sent: right";
            break;
		case RC_PLAY:
			irCode = 0xE0E0E21D; // play
			message = "sent: play";	
			break;
		case RC_PAUSE:
			irCode = 0xE0E052AD; // pause
			message = "sent: pause";
			break;
        default:
            Serial.println("Invalid key");
            return;
    }

    if (message) {
        Serial.println(message);
    }
    IrSender.sendSAMSUNG(irCode, 32);
    delay(kTransmissionDelay);
}
