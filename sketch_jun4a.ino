#include <Servo.h>

Servo left_motor;  // 白色：1510
Servo right_motor; // 红色：1510
Servo claw_servo;  // 机械爪

//蜂鸣器
int beep_pin = 11;

// 机械爪
int servo_pin = 3;
int claw_open = 140;
int claw_close = 80;

// 电机引脚
int right_state = 0;
int left_state = 0;
int left_pin = 10;
int right_pin = 9;
int left_stop = 1510;
int right_stop = 1510;

int speed = 500;
int left_forward = left_stop + speed;
int left_backward = left_stop - speed;
int right_forward = right_stop - speed;
int right_backward = right_stop + speed;

int right_detect = 12;
int left_detect = 13;

int count = 0;

void setup()
{
  claw_servo.attach(servo_pin);
  ClawOpen();
  left_motor.attach(left_pin);
  right_motor.attach(right_pin);
  pinMode(12, INPUT);
  pinMode(13, INPUT);
  Serial.begin(9600);
  pinMode(beep_pin,OUTPUT);
  digitalWrite(beep_pin,HIGH);
  delay(300);
  digitalWrite(beep_pin,LOW);
}

void TraceToDoubleBlack(bool forward = true, String state = "")
{
  while (true)
  {
    right_state = digitalRead(right_detect);
    left_state = digitalRead(left_detect);
    // Serial.println(right);
    // Serial.println(left);
    // Serial.println();

    if (right_state == 0 && left_state == 0)
    {
      if (forward)
        Forward();
      else
        Backward();
    }
    else if (right_state == 1 && left_state == 0)
    {
      if (forward)
        TurnRight();
      else
        TurnLeft();
    }
    else if (right_state == 0 && left_state == 1)
    {
      if (forward)
        TurnLeft();
      else
        TurnRight();
    }
    else
      break;
  }
  Stop();
  // delay(1000);
  Serial.println(state);
  digitalWrite(beep_pin,HIGH);
  delay(300);
  digitalWrite(beep_pin,LOW);
}

void work(){
  TraceToDoubleBlack(true, "物块位置");
  ClawClose();
  Forward();
  delay(500);
  Stop();

  TraceToDoubleBlack(true, "临时位置");
  TurnRight();
  delay(960);
  Stop();

  TraceToDoubleBlack(true, "物块位置");
  Forward();
  delay(500);
  Stop();

  TraceToDoubleBlack(true, "分叉点");
  Forward();
  delay(500);
  Stop();
  TurnLeft();
  delay(1300);
  Stop();
  Forward();
  delay(300);
  Stop();

  TraceToDoubleBlack(true, "左物块位置");
  Forward();
  delay(500);
  Stop();

  TraceToDoubleBlack(true, "目标位置");
  ClawOpen();
  Backward();
  delay(300);
  Stop();

  TurnLeft();
  delay(960);
  Stop();

  TraceToDoubleBlack(true, "左物块位置");
  Forward();
  delay(500);
  Stop();
}

void loop()
{
  TraceToDoubleBlack(true, "起始点");
  Forward();
  delay(500);
  Stop();

  TraceToDoubleBlack(true, "分叉点");
  Forward();
  delay(500);
  Stop();

  for (int i = 1; i <= 2; i++)
  {
    Serial.println("===================================");
    Serial.print("第");
    Serial.print(i);
    Serial.println("次循环");

    work();

    TraceToDoubleBlack(true, "分叉点");
    Forward();
    delay(500);
    Stop();
    TurnLeft();
    delay(500);
    Stop();
    Forward();
    delay(300);
    Stop();
  }

  work();

  TraceToDoubleBlack(true, "分叉点");
  Forward();
  delay(500);
  Stop();
  TurnLeft();
  delay(250);
  Stop();
  Forward();
  delay(300);
  Stop();


  TraceToDoubleBlack(true, "home");
  Forward();
  delay(750);
  Stop();
  digitalWrite(beep_pin,HIGH);
  delay(300);
  digitalWrite(beep_pin,LOW);


  while (true)
    ;
}

void Forward()
{
  left_motor.writeMicroseconds(left_forward);
  right_motor.writeMicroseconds(right_forward);
}
void Backward()
{
  left_motor.writeMicroseconds(left_backward);
  right_motor.writeMicroseconds(right_backward);
}
void TurnRight()
{
  left_motor.writeMicroseconds(left_forward);
  right_motor.writeMicroseconds(right_backward);
}
void TurnLeft()
{
  left_motor.writeMicroseconds(left_backward);
  right_motor.writeMicroseconds(right_forward);
}
void Stop()
{
  left_motor.writeMicroseconds(left_stop);
  right_motor.writeMicroseconds(right_stop);
}

void ClawOpen()
{
  claw_servo.write(claw_open);
  delay(1000);
}
void ClawClose()
{
  claw_servo.write(claw_close);
  delay(1000);
}