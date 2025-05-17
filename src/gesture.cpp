#include "gesture.h"
#include "samsung-remote.h"
#include "sonar.h"
#include "RevEng_PAJ7620.h"
enum state_tv tv_state = TV_OFF;
enum state_tv tv_last_state = TV_OFF;

void switch_tv_state(enum state_tv new_state)
{
	if (tv_state != new_state)
	{
		tv_last_state = tv_state;
		tv_state = new_state;
	}
	Serial.print("TV state: ");
	Serial.println(StateTvToString(tv_state));	
}

const char* GestureToString(Gesture gesture) 
{
    switch (gesture) {
        case GES_NONE:return "None";
        case GES_UP:return "Up";
        case GES_DOWN:return "Down";
        case GES_LEFT:return "Left";
        case GES_RIGHT: return "Right";
        case GES_FORWARD: return "Forward";
        case GES_BACKWARD:return "Backward";
        case GES_CLOCKWISE: return "Clockwise";
        case GES_ANTICLOCKWISE: return "Anticlockwise";
        case GES_WAVE:return "Wave";
        default:return "Unknown";
    }
}


RemoteCommand Gesture2RemoteCommand(Gesture gesture)
{

    switch (gesture)
    {
//		case GES_BACKWARD:
//			return RC_OFF;
        case GES_LEFT:
            if (tv_state == YT_JUMPING) {
                switch_tv_state(YT_PLAYING);
                return RC_ENTER;
            } else if (tv_state == YT_PLAYING)
            {
                switch_tv_state(TV_ON);
                return RC_RETURN;
            } else 
                return RC_LEFT;

        case GES_RIGHT:
            if (tv_state == TV_OFF) {
                switch_tv_state(TV_ON);
                return RC_ON;
            } else if (tv_state == YT_PLAYING) {
                switch_tv_state(YT_JUMPING);
                return RC_RIGHT;
            } else {
                return RC_RIGHT;
            }
        case GES_UP:
            return RC_UP;
        case GES_DOWN:
            return RC_DOWN;
        case GES_CLOCKWISE:
        case GES_FORWARD:
            if (tv_state == TV_ON)
            {
                switch_tv_state(YT_PLAYING);
                return RC_ENTER;
            } else 
                return RC_INVALID_KEY;
        case GES_ANTICLOCKWISE:
            switch_tv_state(TV_OFF);
            return RC_OFF;
        default:
            return RC_INVALID_KEY;
    }

}

const char* StateTvToString(enum state_tv state)
{
    switch(state) {
        case TV_OFF:      return "TV_OFF";
        case TV_ON:       return "TV_ON";
        case YT_PLAYING:  return "YT_PLAYING";
        case YT_PAUSE:    return "YT_PAUSE";
        case YT_JUMPING:  return "YT_JUMPING";
        default:          return "UNKNOWN";
    }
}