// Includes enum definition of GES_* return values from readGesture()
#include "RevEng_PAJ7620.h"
#include "samsung-remote.h"
#include "sonar.h"
// Create gesture sensor driver object
RevEng_PAJ7620 sensor = RevEng_PAJ7620();

#define PIN_RED_LED PD4
#define PIN_GREEN_LED PD7
// *********************************************************************
void setup()
{
	Serial.begin(115200);

	Serial.println("Samsung TV Remote Control with Gesture Sensor");
		
	sonar_init();
	Serial.println("sonar init: done");
	
	// Initialize the gesture sensor
	if( !sensor.begin() ) // return value of 0 == success
	{
		Serial.print("PAJ7620 I2C error - halting");
		while(true) { }
	}	
	Serial.println("PAJ7620 init: OK");
	pinMode(PIN_RED_LED, OUTPUT); // Set pin 5 as output for RED LED
	pinMode(PIN_GREEN_LED, OUTPUT); // Set pin 6 as output for GREEN LED
	pinMode(IR_SEND_PIN, OUTPUT); // Set pin 3 as output for IR LED

	digitalWrite(PIN_RED_LED, HIGH);
	digitalWrite(PIN_GREEN_LED, HIGH);
	delay(3000);
	digitalWrite(PIN_RED_LED, LOW);
	digitalWrite(PIN_GREEN_LED, LOW);


	IRinit();
	Serial.println("IR init: OK");


}


const char* GestureToString(Gesture gesture) {
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
bool isTVon = false;
bool isPlaying = false;	
bool isPause = false;
bool isJumping = false;
RemoteCommand Gesture2RemoteCommand(Gesture gesture)
{

	switch (gesture)
	{
//		case GES_FORWARD:
//			return RC_ON;
//		case GES_BACKWARD:
//			return RC_OFF;
		case GES_LEFT:
			if (isJumping) {
				isJumping = false;
				return RC_ENTER;
			} else if (isPlaying)
			{
				isPlaying = false;
				return RC_RETURN;
			} else {
				return RC_LEFT;
			}
		case GES_RIGHT:
			if (!isTVon) {
				isTVon = true;
				return RC_ON;
			} else if (isPlaying) {
				isJumping = true;
				return RC_RIGHT;
			} else {
				return RC_RIGHT;
			}
		case GES_UP:
			return RC_UP;
		case GES_DOWN:
			return RC_DOWN;
		case GES_CLOCKWISE:
			isPlaying = true;
			return RC_ENTER;
		case GES_ANTICLOCKWISE:
			isPlaying = false;
			isTVon = false;
			return RC_OFF;
		default:
			return RC_INVALID_KEY;
	}

}
void led_allon()
{
	digitalWrite(PIN_RED_LED, HIGH);
	digitalWrite(PIN_GREEN_LED, HIGH);
}
void led_flash_green()
{
	digitalWrite(PIN_GREEN_LED, HIGH);
	delay(150);
	digitalWrite(PIN_GREEN_LED, LOW);
	delay(50);
}
void led_flash_red()
{
	digitalWrite(PIN_RED_LED, HIGH);
	delay(150);
	digitalWrite(PIN_RED_LED, LOW);
	delay(50);
}

int cn_tobe_pause=0;
unsigned long tm_lastjump=0, tm_lastpause=0;
int intv_jump=250; // 250 ms
#define CN_LEAVE 6
// *********************************************************************
void loop()
{
	RemoteCommand cmd;
	Gesture gesture;// Gesture is an enum type from RevEng_PAJ7620.h
	gesture = sensor.readGesture(); // Read back current gesture (if any) of type Gesture

	if (gesture != GES_NONE) 
	{
		Serial.print("Gesture detected: ");
		Serial.println(GestureToString(gesture));
		
		cmd=Gesture2RemoteCommand(gesture);
		sendIRCommand(cmd);
		if (cmd == RC_ON)
		{
			digitalWrite(PIN_RED_LED, HIGH); // Turn on the red LED before delaying for smooth user experience
			delay(3000);
		 	sendIRCommand(RC_ENTER); // for my samsung TV, need to send enter after on to enter youtube
		}	

		if (isTVon)
		{
			digitalWrite(PIN_RED_LED, HIGH);
		} else {
			digitalWrite(PIN_RED_LED, LOW);
		}
		if (isPlaying)
		{
			digitalWrite(PIN_GREEN_LED, HIGH);
		} else {
			digitalWrite(PIN_GREEN_LED, LOW);
		}

	} else if (isJumping) {

		if (((int)(millis() - tm_lastjump)) >= intv_jump) {
			tm_lastjump = millis();		
			Serial.println("Jumping");
			cmd = RC_RIGHT;
			sendIRCommand(cmd);
		}
		
	}
	
	
	int distance = checkDistance();
	if (distance > 120)
	{
		led_flash_green();
		cn_tobe_pause = (cn_tobe_pause<CN_LEAVE)?cn_tobe_pause+1:cn_tobe_pause;			
		
		if ((cn_tobe_pause == CN_LEAVE)&&(isPlaying))
		{
			isPlaying = false;
			isPause=true;
			tm_lastpause = millis();
			sendIRCommand(RC_PAUSE);
			Serial.println("pause");
		} 
		//Serial.println(millis()-tm_lastpause);
	} else if (distance>0) {
		cn_tobe_pause = (cn_tobe_pause>0)?cn_tobe_pause-1:cn_tobe_pause;			
		if (isPause)
			led_flash_red();
		if ((cn_tobe_pause == 0)&&(isPause==true))
		{
			isPlaying = true;
			isPause=false;
			sendIRCommand(RC_PLAY);
			Serial.println("play");
			led_allon();
		}
	}


	if (((millis()-tm_lastpause) > 120000)&&(isTVon==true))
	{
		isTVon = false;
		isPlaying = false;
		isPause = false;
		cn_tobe_pause = 0;
		digitalWrite(PIN_RED_LED, LOW);
		digitalWrite(PIN_GREEN_LED, LOW);
		sendIRCommand(RC_OFF);
		Serial.println("TV off");
	}
	delay(100);	
}