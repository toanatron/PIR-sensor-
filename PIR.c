#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>


#define BTN_SPEED_INCREASE	2
#define BTN_SPEED_DECREASE 	3
#define PIR_OUT_PIN 		21
#define MotorPin_A		4
#define MotorPin_B		5



int speed = 51;

void motor(int speed){
	//run
	//counter-clockwise
	digitalWrite(MotorPin_A, LOW);
	softPwmWrite(MotorPin_B, speed);
	delay(1000);
	 //stop
	digitalWrite(MotorPin_A,HIGH);
	digitalWrite(MotorPin_B,HIGH);
}

void buttonScan(void){
	if(digitalRead(BTN_SPEED_INCREASE) == 0){
		delay (10);
		if(digitalRead(BTN_SPEED_INCREASE) == 0){
			speed++;
			if(speed > 100){
				speed = 100;
			}
			printf("speed = %d\n", speed);
		}
		while(!digitalRead(BTN_SPEED_INCREASE));
	}

	if(digitalRead(BTN_SPEED_DECREASE) == 0){
		delay(10);
		if(digitalRead(BTN_SPEED_DECREASE) == 0){
			speed--;
			if(speed < 0){
				speed = 0;
			}
			printf("speed = %d\n",speed);
		}
		while(!digitalRead(BTN_SPEED_DECREASE));
	}
}

int main(void){
	if(wiringPiSetup() == -1){
		printf("setup wiringPi failed!\n");
		return -1;
	}

	pinMode(MotorPin_A, OUTPUT);
	pinMode(MotorPin_B,OUTPUT);
	
	pinMode(BTN_SPEED_DECREASE, INPUT);
	pinMode(BTN_SPEED_INCREASE, INPUT);
	
	pinMode(PIR_OUT_PIN, INPUT);

	pullUpDnControl(BTN_SPEED_DECREASE, PUD_UP);
	pullUpDnControl(BTN_SPEED_INCREASE, PUD_UP);

	softPwmCreate(MotorPin_B, 0, 100);
	digitalWrite(MotorPin_A,HIGH);
	digitalWrite(MotorPin_B,HIGH);	
	while(1){
		if( 0 == digitalRead(PIR_OUT_PIN)){
			printf("Movement not detected!\n");			
			digitalWrite(MotorPin_A,HIGH);
			digitalWrite(MotorPin_B,HIGH);			
		}else{
			printf("Movement detected!\n");
			buttonScan();
			motor(speed);
		}
	}
	return 0;
}

