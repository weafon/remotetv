// 這些都是跟AI一起隨便寫的醜CODE 請不要當作weafon的寫作標準喔

#include "RevEng_PAJ7620.h"
#include "samsung-remote.h"
#include "sonar.h"
#include "gesture.h"


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


	} 
	switch(tv_state)
	{
		case TV_ON:
			digitalWrite(PIN_RED_LED, HIGH);
			break;
		case TV_OFF:
			digitalWrite(PIN_RED_LED, LOW);
			digitalWrite(PIN_GREEN_LED, LOW);
			break;
		case YT_PLAYING:
			digitalWrite(PIN_GREEN_LED, HIGH);
			digitalWrite(PIN_RED_LED, HIGH);
			break;
		case YT_MENU:
			digitalWrite(PIN_RED_LED, HIGH);
			digitalWrite(PIN_GREEN_LED, LOW);
			break;
		case YT_PAUSE:
			digitalWrite(PIN_GREEN_LED, LOW);
			digitalWrite(PIN_RED_LED, HIGH);
			break;
		case YT_JUMPING:
			if (((int)(millis() - tm_lastjump)) >= intv_jump) {
				tm_lastjump = millis();		
				Serial.println("Jumping");
				cmd = RC_RIGHT;
				sendIRCommand(cmd);
			}
			break;

	}
	delay(100);	
	if (tv_state == TV_OFF)
		return;
	
	int distance = checkDistance();
	if (distance > 120)
	{
		if (cn_tobe_pause<CN_LEAVE)
		{
			if ((tv_state == YT_PLAYING)||(tv_state == YT_MENU))
				led_flash_red();
			cn_tobe_pause ++;
		}
		if ((cn_tobe_pause == CN_LEAVE)&&(tv_state == YT_PLAYING))
        {
            switch_tv_state(YT_PAUSE);
            tm_lastpause = millis();
            sendIRCommand(RC_PAUSE);
            Serial.println("pause");
        } 
    } else if (distance>0) {
        cn_tobe_pause = (cn_tobe_pause>0)?cn_tobe_pause-1:cn_tobe_pause;
        if (tv_state == YT_PAUSE)
            led_flash_green();
        if ((cn_tobe_pause == 0)&&(tv_state == YT_PAUSE))
        {
            switch_tv_state(YT_PLAYING);
            sendIRCommand(RC_PLAY);
            Serial.println("play");
        }
    }


    if (((millis()-tm_lastpause) > 120000)&&(cn_tobe_pause == CN_LEAVE))
    {
        switch_tv_state(TV_OFF);
        cn_tobe_pause = 0;
        sendIRCommand(RC_OFF);
        Serial.println("TV off");
    }

}