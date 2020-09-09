/*
  Скетч к проекту "Телега на всенаправленных колёсах"
  - Страница проекта (схемы, описания): https://alexgyver.ru/mecanumbot
  - Исходники на GitHub: https://github.com/AlexGyver/MecanumBot/
  Проблемы с загрузкой? Читай гайд для новичков: https://alexgyver.ru/arduino-first/
  Нравится, как написан код? Поддержи автора! https://alexgyver.ru/support_alex/
  AlexGyver, AlexGyver Technologies, 2020
  https://www.youtube.com/c/alexgyvershow
  https://github.com/AlexGyver
  https://AlexGyver.ru/
  alex@alexgyver.ru
*/

#define MAX_SPEED 255   // максимальная скорость моторов (0-255)
#define MIN_DUTY 100

#define MOTOR_TEST 0    // тест моторов
// при запуске крутятся ВПЕРЁД по очереди:
// FL - передний левый
// FR - передний правый
// BL - задний левый
// BR - задний правый

// пины драйверов (_B должен быть ШИМ)
#define MOTOR1_A 2
#define MOTOR1_B 3  // ШИМ!
#define MOTOR2_A 4
#define MOTOR2_B 5  // ШИМ!
#define MOTOR3_A 7
#define MOTOR3_B 6  // ШИМ!
#define MOTOR4_A 8
#define MOTOR4_B 9  // ШИМ!

#include <GyverMotor.h>
// тут можно поменять моторы местами
GMotor motorFL(DRIVER2WIRE, MOTOR1_A, MOTOR1_B, HIGH);
GMotor motorBL(DRIVER2WIRE, MOTOR2_A, MOTOR2_B, HIGH);
GMotor motorBR(DRIVER2WIRE, MOTOR3_A, MOTOR3_B, HIGH);
GMotor motorFR(DRIVER2WIRE, MOTOR4_A, MOTOR4_B, HIGH);

// пины ресивера ps2
#define PS2_DAT A0
#define PS2_CMD A1
#define PS2_SEL A2
#define PS2_CLK A3

#include <PS2X_lib.h>
PS2X ps2x;

void setup() {
  Serial.begin(9600);
  // чуть подразгоним ШИМ https://alexgyver.ru/lessons/pwm-overclock/
  // Пины D3 и D11 - 980 Гц
  TCCR2B = 0b00000100;  // x64
  TCCR2A = 0b00000011;  // fast pwm

  // Пины D9 и D10 - 976 Гц
  TCCR1A = 0b00000001;  // 8bit
  TCCR1B = 0b00001011;  // x64 fast pwm

  // тест моторов
#if (MOTOR_TEST == 1)
  Serial.println("front left");
  motorFL.setMode(AUTO);
  motorFL.setSpeed(255);
  delay(3000);
  motorFL.setSpeed(0);
  motorFL.setMode(STOP);
  delay(1000);
  Serial.println("front right");
  motorFR.setMode(AUTO);
  motorFR.setSpeed(255);
  delay(3000);
  motorFR.setSpeed(0);
  motorFR.setMode(STOP);
  delay(1000);
  Serial.println("back left");
  motorBL.setMode(AUTO);
  motorBL.setSpeed(255);
  delay(3000);
  motorBL.setSpeed(0);
  motorBL.setMode(STOP);
  delay(1000);
  Serial.println("back right");
  motorBR.setMode(AUTO);
  motorBR.setSpeed(255);
  delay(3000);
  motorBR.setSpeed(0);
  motorBR.setMode(STOP);
#endif
  // минимальный сигнал на мотор
  motorFR.setMinDuty(MIN_DUTY);
  motorBR.setMinDuty(MIN_DUTY);
  motorFL.setMinDuty(MIN_DUTY);
  motorBL.setMinDuty(MIN_DUTY);

  // режим мотора в АВТО
  motorFR.setMode(AUTO);
  motorBR.setMode(AUTO);
  motorFL.setMode(AUTO);
  motorBL.setMode(AUTO);

  // скорость плавности
  motorFR.setSmoothSpeed(60);
  motorBR.setSmoothSpeed(60);
  motorFL.setSmoothSpeed(60);
  motorBL.setSmoothSpeed(60);

  delay(1000);    // ждём запуска контроллера
  ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, false, false);
}

void loop() {
  bool success = ps2x.read_gamepad(false, 0);  // читаем
  ps2x.reconfig_gamepad();      // костыль https://stackoverflow.com/questions/46493222/why-arduino-needs-to-be-restarted-after-ps2-controller-communication-in-arduino

  if (success) {    
    byte LX = ps2x.Analog(PSS_LX);
    int valLX; // = map(LX, 0, 256, -MAX_SPEED, MAX_SPEED);
    if (LX < 128 && LX >= 0) { // STICK UP
        valLX = -map(LX, 127, 0, MIN_DUTY, MAX_SPEED);
    } else if (LX > 128 && LX <= 255) { // STICK DOWN
        valLX = map(LX, 129, 255, MIN_DUTY, MAX_SPEED);
    } else  {// STICK CENTER
        valLX = 0;
    }
   
    byte LY = ps2x.Analog(PSS_LY);
    int valLY; // = map(LY, 256, 0, -MAX_SPEED, MAX_SPEED); // инвертируем ось
    if (LY < 128 && LY >= 0) { // STICK UP
        valLY = map(LY, 127, 0, MIN_DUTY, MAX_SPEED);
    } else if (LY > 128 && LY <= 255) { // STICK DOWN
        valLY = -map(LY, 129, 255, MIN_DUTY, MAX_SPEED);
    } else  {// STICK CENTER
        valLY = 0;
    }
    
    byte RX = ps2x.Analog(PSS_RX);
    int valRX; // = map(RX, 0, 256, -MAX_SPEED, MAX_SPEED);
    if (RX < 128 && RX >= 0) { // STICK UP
        valRX = -map(RX, 127, 0, MIN_DUTY, MAX_SPEED);
    } else if (RX > 128 && RX <= 255) { // STICK DOWN                
        valRX = map(RX, 129, 255, MIN_DUTY, MAX_SPEED);
    } else  {// STICK CENTER
        valRX = 0;
    }
    
    byte RY = ps2x.Analog(PSS_RY);
    int valRY; // = map(RY, 256, 0, -MAX_SPEED, MAX_SPEED);
    if (RY < 128 && RY >= 0) { // STICK UP
        valRY = map(RY, 127, 0, MIN_DUTY, MAX_SPEED);
    } else if (RY > 128 && RY <= 255) { // STICK DOWN
        valRY = -map(RY, 129, 255, MIN_DUTY, MAX_SPEED);
    } else  {// STICK CENTER
        valRY = 0;
    }

    int dutyFL = valRY + valRX;
    int dutyFR = valRY - valRX;
    int dutyBL = valRY - valRX;
    int dutyBR = valRY + valRX;    

    dutyFL += valLY + valLX;
    dutyFR += valLY - valLX;
    dutyBL += valLY + valLX;
    dutyBR += valLY - valLX;

    // ПЛАВНЫЙ контроль скорости, защита от рывков
    motorFR.smoothTick(dutyFR);
    motorBR.smoothTick(dutyBR);
    motorFL.smoothTick(dutyFL);
    motorBL.smoothTick(dutyBL);
  } else {
    // страшно, вырубай
    motorFR.setSpeed(0);
    motorBR.setSpeed(0);
    motorFL.setSpeed(0);
    motorBL.setSpeed(0);
  }
  delay(50);
}
