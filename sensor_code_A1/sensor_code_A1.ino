const int US_1_echo = 6;
const int US_1_trig = 5;
const int US_2_sig = 7;
const int US_3_sig = 13;
const int MQ2_1_sig = A5;
const int MQ2_4_sig = A0;
#define DHT11_1_sig 2 //bottom
#define DHT11_2_sig 1 //top
#define DHT11_3_sig 3 //bottom
#define DHT11_4_sig 4 //top

byte read_dht11_dat(int DHT11_PIN){
  byte i = 0;
  byte result=0;
  for(i=0; i< 8; i++)
  {
    while(!(PINC & _BV(DHT11_PIN)));  // wait for 50us
    delayMicroseconds(30);
    if(PINC & _BV(DHT11_PIN)) 
      result |=(1<<(7-i));
    while((PINC & _BV(DHT11_PIN)));  // wait '1' finish
    }
    return result;
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2; //CONFIRM VALUE
}

long pingUS(int pingPin){
  long duration, cm;

  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);
  cm = microsecondsToCentimeters(duration);

  return cm;
}

long HCSR04US(int trigPin, int echoPin){
  
  long duration, cm;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  cm = (duration/2) / 29.1;
  return cm;
  
}

void DHT11(int DHT11_PIN, byte dht11_dat[5]){
  
  byte dht11_in;
  byte i;// start condition

  DDRC |= _BV(DHT11_PIN);
  PORTC |= _BV(DHT11_PIN);
  
   // 1. pull-down i/o pin from 18ms
  PORTC &= ~_BV(DHT11_PIN);
  //Serial.println(PORTC);
  delay(18);
  PORTC |= _BV(DHT11_PIN);
  delayMicroseconds(40);
  DDRC &= ~_BV(DHT11_PIN);
  delayMicroseconds(40);
  
  dht11_in = PINC & _BV(DHT11_PIN);
  if(dht11_in)
  {
    Serial.println("dht11 start condition 1 not met");
    //Serial.println(DDRC);
    //Serial.println(PORTC);
    return;
  }
  delayMicroseconds(80);
  dht11_in = PINC & _BV(DHT11_PIN);
  if(!dht11_in)
  {
    Serial.println("dht11 start condition 2 not met");
    return;
  }
  
  delayMicroseconds(80);// now ready for data reception
  for (i=0; i<5; i++)
    dht11_dat[i] = read_dht11_dat(DHT11_PIN);
  DDRC |= _BV(DHT11_PIN);
  PORTC |= _BV(DHT11_PIN);
  byte dht11_check_sum = dht11_dat[0]+dht11_dat[1]+dht11_dat[2]+dht11_dat[3];// check check_sum
  if(dht11_dat[4]!= dht11_check_sum)
  {
    Serial.println("DHT11 checksum error");
  }

  


}

void setup(){

  Serial.begin(9600);
//  Serial.println("Ready");

  pinMode(US_1_trig, OUTPUT);
  pinMode(US_1_echo, INPUT);
}

void loop(){

  long cm1, cm2, cm3;
  int gas1, gas4;
  byte dht11_1_dat[5], dht11_2_dat[5], dht11_3_dat[5], dht11_4_dat[5];
  
  //US_1
  cm1 = HCSR04US(US_1_trig, US_1_echo);
  Serial.print("US1 "); 
  Serial.print(cm1);
  Serial.print(",");
  
  //US_2
  cm2 = pingUS(US_2_sig);
  Serial.print("US2 "); 
  Serial.print(cm2);
  Serial.print(",");

  //US_3
  cm3 = pingUS(US_3_sig); 
  Serial.print("US3 ");
  Serial.print(cm3);
  Serial.print(",");

  //MQ2_1
  gas1 = analogRead(MQ2_1_sig);
  Serial.print("GAS1 ");
  Serial.print(gas1,DEC);
  Serial.print(",");

  //MQ2_4
  gas4 = analogRead(MQ2_4_sig);
  Serial.print("GAS4 ");
  Serial.print(gas4,DEC);
  Serial.print(",");

  //DHT11_1_sig
  DHT11(DHT11_1_sig, dht11_1_dat);
  Serial.print("Temp1 ");
//  Serial.print("Current humdity = ");
//  Serial.print(dht11_1_dat[0], DEC);
//  Serial.print(".");
//  Serial.print(dht11_1_dat[1], DEC);
//  Serial.print("%  ");
//  Serial.print("temperature = ");
  Serial.print(dht11_1_dat[2], DEC);
  Serial.print(".");
  Serial.print(dht11_1_dat[3], DEC);
  Serial.print(" ");
//  Serial.println("C  ");



  //DHT11_2_sig
  DHT11(DHT11_2_sig, dht11_2_dat);
//  Serial.print("DHT_11_2 ");
//  Serial.print("Current humdity = ");
//  Serial.print(dht11_2_dat[0], DEC);
//  Serial.print(".");
//  Serial.print(dht11_2_dat[1], DEC);
//  Serial.print("%  ");
//  Serial.print("temperature = ");
  Serial.print(dht11_2_dat[2], DEC);
  Serial.print(".");
  Serial.print(dht11_2_dat[3], DEC);
  Serial.print(",");
//  Serial.println("C  ");


  //DHT11_3_sig
  DHT11(DHT11_3_sig, dht11_3_dat);
  Serial.print("Temp2 ");
//  Serial.print("Current humdity = ");
//  Serial.print(dht11_3_dat[0], DEC);
//  Serial.print(".");
//  Serial.print(dht11_3_dat[1], DEC);
//  Serial.print("%  ");
//  Serial.print("temperature = ");
  Serial.print(dht11_3_dat[2], DEC);
  Serial.print(".");
  Serial.print(dht11_3_dat[3], DEC);
  Serial.print(" ");
//  Serial.println("C  ");


  //DHT11_4_sig
  DHT11(DHT11_4_sig, dht11_4_dat);
//  Serial.print("DHT_11_4 ");
//  Serial.print("Current humdity = ");
//  Serial.print(dht11_4_dat[0], DEC);
//  Serial.print(".");
//  Serial.print(dht11_4_dat[1], DEC);
//  Serial.print("%  ");
//  Serial.print("temperature = ");
  Serial.print(dht11_4_dat[2], DEC);
  Serial.print(".");
  Serial.print(dht11_4_dat[3], DEC);
  Serial.println();
//  Serial.println("C  ");

  delay(3000);
}

