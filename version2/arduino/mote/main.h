/*
 * main.h
 * Author: Tada Matz
 * Comment: define pins and interval
 */

#ifndef MAIN_H
#define MAIN_H

//button
#define BUTTON_PIN 9

//Click LED
#define CLICK_LED_PIN 3
#define CLICK_LED_ON_INTERVAL 1000

//Receive LED
#define RECEIVE_LED_PIN 2
#define RECEIVE_LED_ON_INTERVAL 1000

//packet
#define ID_PACKET_OFFSET '0'
#define UPLINK_HEADER 'U'
#define DOWNLINK_HEADER 'D'
#define LED_INSTRUCTION 'L'
#define SERVO_INSTRUCTION 'S'

//temperature sensor
#define TEMP_SENSOR_PIN A0
#define SERIESRESISTOR 10000

//volume
#define VOLUME_PIN A1

//Servo
#define SERVO_PIN 10
#define SERVO_ON_INTERVAL 5000

#endif
