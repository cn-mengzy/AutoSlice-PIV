/*
 * AutoSlice-PIV - Arduino Stepper Motor Control.
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
 * Author: [Ziyu Meng]
 * Company:[University of Bristol]
 * Year: [2024]
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

long stepPosition, maxPosition; 
int motorSpeed =300;

long matchNumber(String command, String keyword) {
    int c_start = command.indexOf(keyword);
        
    if (c_start != -1) {
//        Serial.println(command);
//        Serial.println(keyword);
        
        c_start += keyword.length();  // 移动到关键字之后的数字的开始位置
        
        int c_end = c_start;            // 初始化结束位置
        
        // 找到数字部分的结束位置
        while (c_end < command.length() && isDigit(command[c_end])) {
            c_end++;
        }

        String numStr = command.substring(c_start, c_end);
        
        // 使用strtol从字符串中解析长整型
        char* endPtr;
        long num = strtol(numStr.c_str(), &endPtr, 10);
        
        if (*endPtr == '\0') {  // 确保整个字符串都是数字
            return num;
        }
    }
    return -1;  // 如果解析失败或找不到关键字，则返回-1
}

void showMenu() {
    Serial.println("Command Menu:");
    Serial.println("1. Reset Home Position");
    Serial.println("2. Move Forward s steps");
    Serial.println("3. Move Backward s steps");
    Serial.println("4. Go to Position V");
    Serial.println("5. Go to End Position");
    Serial.println("6. Reply OK");
    Serial.println("7. Set Motor Speed (RPM)");
    Serial.println("8. Ask Motor Speed");
    Serial.println("9. Ask Current Position");
    Serial.println("Enter a command (1-9):");
}

void setup() {
    pinMode(pin_Endstop1, INPUT_PULLUP); 
    pinMode(pin_Endstop2, INPUT_PULLUP);
    steppermotor.setSpeed(motorSpeed); 
    Serial.begin(115200);
    Serial.println("Ready");
}



void loop() {
  // put your main code here, to run repeatedly:
   if (Serial.available()){
      char message[32];
      size_t bytesRead = Serial.readBytesUntil('\n', message, 32);
      message[bytesRead] = '\0'; // Add null-terminator

      int c = matchNumber(message, "c");

      if (c>=0 && c<3){
      switch (c) {
        case 0:     
                  showMenu();
                  break;

        case 1:     // detect home position and max position
                  steppermotor.setSpeed(1000);
                  while (digitalRead(pin_Endstop1)) { 
                              steppermotor.step(-1); 
                          }
                  stepPosition = 0; 

                  while (digitalRead(pin_Endstop2)) { 
                              steppermotor.step(1); 
                               stepPosition++;
                          }
                  maxPosition = stepPosition;
                  steppermotor.setSpeed(motorSpeed);
                  break;

        case 2:   // move forward s step, if send c2s, then1 s =1 and move 1 step forward
                  long s = matchNumber(message, "s");
                  long targetForwardPosition = stepPosition+s;
                  if (targetForwardPosition<= maxPosition){
                  steppermotor.step(s);
                  stepPosition = stepPosition+s;}
                  else if (targetForwardPosition>maxPosition){
                    Serial.println("target position exceed max");
                  }
                  break;
        default:
                  Serial.println("Invalid command");
                  break;
      }}
      else if (c==3){
                  
                  long s = -matchNumber(message, "s");
                  long targetMPosition = stepPosition+s;
                  if (targetMPosition>=0 && targetMPosition<=maxPosition){
                  steppermotor.step(s);
                  stepPosition = stepPosition+s;}
      }
      else if (c==4){
       // go to postion V; if v = 100, then go to v =100 step;
                  long v = matchNumber(message, "v");
                 
                  if (v>=0 && v<=maxPosition){
                     long stepsToMove = v - stepPosition; // calculate the step need to move in total
                     const int maxStep = 30720;
                     // 2048step = 360 degree 2048*15=30720
                     //if the motor need to move more than 15 circle then the cmd need to excute in multi steps
                     //15 circle should be 15*0.5mm=7.5mm(need to verify again)

                      // multi-step system
                      while (stepsToMove != 0) {
                          if (stepsToMove > maxStep) {
                              steppermotor.step(maxStep);   // 正向最大步数
                              stepsToMove -= maxStep;
                          } else if (stepsToMove < -maxStep) {
                              steppermotor.step(-maxStep);  // 反向最大步数
                              stepsToMove += maxStep;
                          } else {
                              steppermotor.step((int)stepsToMove); // 剩余步数
                              stepsToMove = 0;
                          }
                      }
                  stepPosition = v;}
      }
      else if (c==5){     // go to end position
                  long stepCount=0;
                  while (digitalRead(pin_Endstop2)) { 
                              steppermotor.step(1); 
                              stepCount++;
                          }
                  stepPosition = stepPosition + stepCount;
                  maxPosition = stepPosition;
      }
      else if (c==6){     // reply ok
                  Serial.println("ok");
      }
       else if (c==7){     // setspeed rpms
                  motorSpeed = matchNumber(message, "v");
                  if (motorSpeed>=100 & motorSpeed <=1000){
                  steppermotor.setSpeed(motorSpeed);}
                  else{
                    Serial.println("speed should between 100 to 1000 rpm");
                  }
       }
       else if (c==8){    // ask speed
                  Serial.println(motorSpeed);
       }
       else if (c==9){     // ask position
                  Serial.println(stepPosition);
       }
       else if (c==10){     // ask position
                  Serial.println(maxPosition);
   }
}}
