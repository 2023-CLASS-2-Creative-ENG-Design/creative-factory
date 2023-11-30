#include <Servo.h> 

#define SERVOPINH   5           // 수평 서보 모터
#define SERVOPINV   6           // 수직 서보 모터
#define DELAYTIME   50          // delay constant
#define THRESHOLD   50          // threshold

Servo horizontal;               // 수평 서보
int servo_h = 90;               // default
int servo_h_limit_high = 175;   // 수평 최대 각도
int servo_h_limit_low = 5;      // 수평 최소 각도

Servo vertical;                 // 수직 서보
int servo_v = 90;               // default
int servo_v_limit_high = 100;   // 수직 최대 각도
int servo_v_limit_low = 20;     // 수직 최소 각도

const int light_ul = A0;        //Upper Left
const int light_ur = A1;        //Upper Right
const int light_ll = A2;        //Lower Left
const int light_lr = A3;        //Lower Right

void setup() {
  Serial.begin(9600);
  horizontal.attach(SERVOPINH);
  vertical.attach(SERVOPINV);
  horizontal.write(servo_h);
  vertical.write(servo_v);
  delay(100);
}

void loop()  { 
  // 조도 센서의 값을 읽어옴
  int ul = analogRead(light_ul); 
  int ur = analogRead(light_ur); 
  int ll = analogRead(light_ll); 
  int lr = analogRead(light_lr); 
  
  // 조도센서의 상하좌우 평균값을 구함
  int ave_upper = (ul + ur) / 2; // up_l_r
  int ave_lower = (ll + lr) / 2; // low_l_r
  int ave_left  = (ul + ll) / 2; // left_up_low
  int ave_right = (ur + lr) / 2; // right_up_low Average the adjacent readings
  
  int diff_v  = ave_upper - ave_lower; // 상하 차이
  int diff_h = ave_left  - ave_right; // 좌우 차이

  // diff_v의 절댓값이  THRESHOLD보다 크다면 수직 서보 모터를 돌림
  if (-1 *  THRESHOLD > diff_v || diff_v > THRESHOLD) {
    if (ave_upper < ave_lower)
      if(servo_v <= servo_v_limit_high) 
        servo_v++;
    else
      if(servo_v >= servo_v_limit_low)
        servo_v--;
    vertical.write(servo_v);
  }

  // diff_h의 절댓값이  THRESHOLD보다 크다면 수평 서보 모터를 돌림
  if (-1 *  THRESHOLD > diff_h || diff_h > THRESHOLD) {
    if (ave_left < ave_right)
      if (servo_h >= servo_h_limit_low)
        servo_h--; 
    else
      if (servo_h <= servo_h_limit_high)
        servo_h++;
    horizontal.write(servo_h);
  }
  
  delay(DELAYTIME);
}