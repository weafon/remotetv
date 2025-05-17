#ifndef GESTURE_H
#define GESTURE_H	
#include "RevEng_PAJ7620.h"
#include "samsung-remote.h"

enum state_tv
{
	TV_OFF,
	TV_ON,
	YT_PLAYING, // isPlaying Youtube
	YT_PAUSE, // isPause Youtube
	YT_JUMPING, // isJumping Youtube
};
const char* StateTvToString(enum state_tv state);
extern enum state_tv tv_state, tv_last_state;
const char* GestureToString(Gesture gesture);
RemoteCommand Gesture2RemoteCommand(Gesture gesture);
void switch_tv_state(enum state_tv new_state);
#endif