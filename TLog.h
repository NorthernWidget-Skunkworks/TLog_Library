
#ifndef TLOG_h
#define TLOG_h

#include "Arduino.h"
#include <Wire.h>
#include <SPI.h>
#include <DS3231_Logger.h>
#include <SdFat.h>
#include <avr/pgmspace.h>
#include <EEPROM.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <LTC2495.h>
#include <SparkFunSX1509.h> 


#define RED 0xFFFF0000L
#define GREEN 0xFF00FF00L
#define BLUE 0xFF0000FFL
#define MAROON 0xFF800000L
#define GOLD 0xFFFFD700L
#define ORANGE 0xFFFFA500L
#define PURPLE 0xFF800080L
#define CYAN 0xFF00FFFF
#define BLACK_ALERT 0x802019FF
#define OFF 0x00

////////////////////////////PIN DEFINITIONS///////////////////////





class TLog
{

	public:
		TLog();
		int begin(uint8_t *Vals, uint8_t NumVals, String Header_);
		int begin(String Header_);

		int LogStr(String Val);
		void LED_Color(unsigned long Val);
		void Run(String (*f)(void), unsigned long LogInterval);
		// float analogRead(uint8_t Pin);
		// uint8_t digitalRead(uint8_t Pin);
		// void digitalWrite(uint8_t Pin, uint8_t State);
	protected:
		float TempConvert(float V, float Vcc, float R, float A, float B, float C, float D, float R25);
		void Blink();
		// void StartLog();
		// void Log();
		void virtual Log();
		void virtual StartLog();
		static void isr0();
		static void isr1();
		static TLog* selfPointer;

		void sleepNow();
		void turnOffSDcard();
		void turnOnSDcard();
		void GetTime();
		String GetOnBoardVals();
		void I2CTest();
		void SDTest();
		void ClockTest();
		void PowerTest();
		void InitLogFile();

		DS3231_Logger RTC;
		SX1509 io; // Create an SX1509 object to be used throughout
		LTC2495 adc = LTC2495(0x45, 5.0, 15, 1.8);

		#if defined(TLog_1v0)  //FIX DEFINITION!

		#else
			const int SD_CS = 12;
			const uint8_t BuiltInLED = 20;
			const uint8_t RedLED = 5;  //IO Expander
			const uint8_t GreenLED = 7;  //IO Expander
			const uint8_t BlueLED = 6;  //IO Expander

			const uint8_t VRef_Pin = 15;
			const uint8_t BatSense_Pin = 14;

			const uint8_t SD_CD = 1;

			const uint8_t Ext3v3Ctrl = 8; //IO Expander
			const uint8_t Ext5vCtrl = 4; //IO Expander
			const uint8_t SD3v3Ctrl = 2; //IO Expander
			const uint8_t VRefCtrl = 3; //IO Expander
			// const uint8_t I2C_SW = 12;
			// const uint8_t PG = 18;
			const uint8_t ExtInt = A7;
			const uint8_t RTCInt = A6;
			const uint8_t LogInt = A3; 
		#endif

		// float A = 0.003354016;
		// float B = 0.0003074038;
		// float C = 1.019153E-05;
		// float D = 9.093712E-07;
		String LogTimeDate = "2063/04/05 20:00:00";
		// float Temp[5] = {0}; //Temp Downhole, Temp on board, Temp RTC, Temp Baro
		// float Pressure[2] = {0}; //Downhole pressure, Atmospheric pressure (if applicable)
		float BatVoltage = 0;
		bool OBError = false;
		bool SensorError = false;
		bool TimeError = false;
		bool SDError = false; //USE??
		String Header = "";
		uint8_t NumADR = 0;
		uint8_t I2C_ADR[16] = {0}; //Change length??
		const uint8_t NumADR_OB = 4;
		const uint8_t I2C_ADR_OB[4] = {0x68, 0x42, 0x3E, 0x45}; //Clock, GPS, IO Expander, ADC

		volatile bool LogEvent = false; //Used to test if logging should begin yet
		volatile bool NewLog = false; //Used to tell system to start a new log
		volatile int AwakeCount = 0;

		char FileNameC[11]; //Used for file handling
		char FileNameTestC[11]; //Used for file handling
		bool SD_Init = false;
		SdFat SD;
		byte  keep_SPCR;
};

#endif