#define trig1 8
#define echo1 9
#define trig2 10
#define echo2 11
#define pin_in 7
#define buz 2

#define in1 3
#define in2 4
#define in3 12
#define in4 13
#define en1 5
#define en2 6

void setup() {
  // put your setup code here, to run once:
  pinMode(trig1,OUTPUT);
  pinMode(echo1,INPUT);
  pinMode(trig2,OUTPUT);
  pinMode(echo2,INPUT);
  pinMode(pin_in,INPUT);
  pinMode(buz,OUTPUT);
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(in3,OUTPUT);
  pinMode(in4,OUTPUT);
  pinMode(en1,OUTPUT);
  pinMode(en2,OUTPUT);
  Serial.begin(9600);
  // digitalWrite(pin_in,1);
  digitalWrite(pin_in,0);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  int val = digitalRead(pin_in);
  Serial.print("input value: ");
  Serial.println(val);
  int distance1=distread(trig1,echo1);
  int distance2=distread(trig2,echo2);
  Serial.print("distance 1: ");
  Serial.println(distance1);
  Serial.print("distance 2: ");
  Serial.println(distance2);
  if (distance1<=3 && distance2<=3){
    if (pin_in==1){
      int i=3;
      while (i){
        forward();
        analogWrite(en1,200);
        analogWrite(en2,200);
        digitalWrite(buz,1);
        delay(150);
        digitalWrite(buz,0);
        i--;
      }
      stop();
      digitalWrite(buz,1);
    }
    else{
      forward();
      analogWrite(en1,220);
      analogWrite(en2,220);
      digitalWrite(buz,0);
    }
  }
  else if (distance1>3 && distance1<6){
    int i=3;
    while (i){
      left();
      analogWrite(en1,180);
      analogWrite(en2,180);
      digitalWrite(buz,1);
      delay(150);
      digitalWrite(buz,0);
      forward();
      analogWrite(en1,180);
      analogWrite(en2,180);
      digitalWrite(buz,1);
      delay(150);
      digitalWrite(buz,0);
      i--;
    }
    stop();
    digitalWrite(buz,1);

  }
  else if (distance1>10 && distance2<10){
    forward();
    analogWrite(en1,220);
    analogWrite(en2,220);
    digitalWrite(buz,0);
  }
  else {
    forward();
  }
  delay(2000);

}

int distread(int trig,int echo){
  digitalWrite(trig,0);
  delayMicroseconds(2);
  digitalWrite(trig,1);
  delayMicroseconds(10);
  digitalWrite(trig,0);
  int duration=pulseIn(echo,1);
  int distance=duration*0.034/2;
  delay(200);
  return distance;
}

void forward(){
  digitalWrite(in1,1);
  digitalWrite(in2,0);
  digitalWrite(in3,1);
  digitalWrite(in4,0);
}

void left(){
  digitalWrite(in1,1);
  digitalWrite(in2,0);
  digitalWrite(in3,1);
  digitalWrite(in4,0);
}

void stop(){
  digitalWrite(in1,0);
  digitalWrite(in2,0);
  digitalWrite(in3,0);
  digitalWrite(in4,0);
}
