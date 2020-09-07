/*
	Пример управления мотором при помощи драйвера полного моста
*/
#include "GyverMotor.h"

/*
  DRIVER2WIRE - двухпроводной драйвер (направление + ШИМ)
  DRIVER3WIRE - трёхпроводной драйвер (два пина направления + ШИМ)
  RELAY2WIRE - реле в качестве драйвера (два пина направления)

  dig_pin, dig_pin_A, dig_pin_B - любой цифровой пин МК
  PWM_pin - любой ШИМ пин МК
  LOW / HIGH - уровень драйвера. Если при увеличении скорости мотор наоборот тормозит - смени уровень
*/
GMotor motor(DRIVER2WIRE, 2, 3, HIGH);


void setup() {
  Serial.begin(9600);
  // установка программного deadtime на переключение направления, микросекунды
  // по умолчанию стоит 0: deadtime отключен
  // motor.setDeadtime(200);

  // ГЛОБАЛЬНАЯ смена направления вращения мотора
  // например чтобы FORWARD совпадал с направлением движения "вперёд" у машинки  
  //motor.setDirection(REVERSE);
  //motor.setDirection(NORMAL);   // умолч.

  // смена режима работы мотора
  //motor.setMode(FORWARD);   // вперёд
  //motor.setMode(BACKWARD);  // назад
  //motor.setMode(STOP);      // холостой (мотор отключен)
  //motor.setMode(BRAKE);     // активное торможение

  // смена уровня драйвера (аналогично при инициализации)
  // Если при увеличении скорости мотор наоборот тормозит - смени уровень
  //motor.setLevel(LOW);
  //motor.setLevel(HIGH);     // по умолч.
  
  // setSpeed в режиме 8-бит (активно по умолчанию)
  // motor.set8bitMode();

  // setSpeed в режиме 10-бит
  // для работы в 10 бит необходимо также настроить ШИМ на 10 бит!!!
  // читай тут https://alexgyver.ru/lessons/pwm-overclock/
  // motor.set10bitMode();

  // минимальный сигнал (по модулю), который будет подан на мотор, то есть либо 0, либо он. 
  // Избавляет от ситуаций, когда мокор покоится и "пищит"
  // а также позволяет сделать мёртвую зону в управлении
  //motor.setMinDuty(150);

  motor.setMode(AUTO); // IMPORTANT!
}

void loop() {
  // установка скорости:
  // * (0..255) при ручном выборе направления (setMode: FORWARD/BACKWARD)
  // * (-255..255) при автоматическом (поставить setMode(FORWARD))
  // * (0..1023) в режиме 10 бит при ручном выборе направления (setMode: FORWARD/BACKWARD)
  // * (-1023..1023) в режиме 10 бит при автоматическом (поставить setMode(FORWARD))    

    for (int i=151; i<255; i+=10) {
      Serial.println(i);
      motor.setSpeed(i);
      delay(500);
    }

    for (int i=255; i>150; i-=10) {
      Serial.println(i);
      motor.setSpeed(i);
      delay(500);
    }

    motor.setSpeed(0);
    delay(300);

    for (int i=-151; i>-255; i-=10) {
      Serial.println(i);
      motor.setSpeed(i);
      delay(500);
    }

    for (int i=-255; i<-150; i+=10) {
      Serial.println(i);
      motor.setSpeed(i);
      delay(500);
    } 
 
  delay(300);  // задержка просто для "стабильности"
}
