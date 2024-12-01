/*
 * AutoSlice-PIV - Arduino Stepper Motor Control
 * 
 * This code is used for controlling a 28BYJ-48 stepper motor using an Arduino Uno.
 * It drives the motor in small steps to control precise movements for the AutoSlice-PIV system.
 * The stepper motor is controlled through the ULN2003 driver module, commonly paired with the 28BYJ-48 motor.
 * 
 * Hardware:
 * - Arduino Uno
 * - 28BYJ-48 Stepper Motor
 * - ULN2003 Driver Module
 * 
 * The motor is used to incrementally scan fluid slices in the AutoSlice-PIV system, enabling detailed flow field analysis.
 * 
 * Author: [Ziyu Meng, University of Bristol]
 * Date: [2024-Dec-01st]
 */
#include <Stepper.h>
#include <SPI.h>

#define pin_Endstop1 12
#define pin_Endstop2 13
#define In1 8
#define In2 9
#define In3 10
#define In4 11

Stepper steppermotor(32, In1, In3, In2, In4); 

int c,f,b;

int matchNumber(String command, String keyword) {
  int start = command.indexOf(keyword);
  if (start != -1) {
    int end = command.indexOf(" ", start);
    String num = command.substring(start + keyword.length(), end);
    return num.toInt();
  }
  return -1;
}

void setup() {
    pinMode(pin_Endstop1, INPUT_PULLUP); // 配置 Endstop1 为输入模式
    steppermotor.setSpeed(100); 
    
    Serial.begin(115200);
    Serial.println("Ready");
    bool trigger = digitalRead(pin_Endstop1);
    Serial.print("trigger=");
    Serial.println(trigger);
}

void loop() {
  // put your main code here, to run repeatedly:
   if (Serial.available()){
      char message[32];
      size_t bytesRead = Serial.readBytesUntil('\n', message, 32);
      message[bytesRead] = '\0'; // Add null-terminator

      int c = matchNumber(message, "c");
      Serial.print("c=");
      Serial.println(c);
      switch (c) {

        case 2:   
                  f = matchNumber(message, "f");
                  Serial.print("f=");
                  Serial.println(f);
                  steppermotor.step(f);
                  break;
        case 3:
                  b = -matchNumber(message, "b");
                  Serial.print("b=");
                  Serial.println(b);
                  steppermotor.step(b);
                  break;
       case 4:
                  while (digitalRead(pin_Endstop1)) { // 检测 Endstop1 的状态
                              steppermotor.step(-1); // 反转步进电机 1 步
                          }
                  break;
       default:
                  Serial.println("Invalid command");
                  break;
      }
   }
}
