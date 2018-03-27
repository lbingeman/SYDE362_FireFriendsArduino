const int US_5_echo = 2;
const int US_5_trig = 3;
const int US_4_sig = 10;
const int MQ2_2_sig = A2;
const int MQ2_3_sig = A3;
#define DHT11_5_sig 5 //top
#define DHT11_6_sig 4 //botoom
#define DHT11_7_sig 1 //bottom
#define DHT11_8_sig 0 //top

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

  pinMode(US_5_trig, OUTPUT);
  pinMode(US_5_echo, INPUT);
}

void loop(){

  long cm4, cm5;
  int gas2, gas3;
  byte dht11_5_dat[5], dht11_6_dat[5], dht11_7_dat[5], dht11_8_dat[5];
  
  //US_4
  cm4 = pingUS(US_4_sig); 
  Serial.print("US4 "); 
  Serial.print(cm4);
  Serial.print(",");
 
  //US_5
  cm5 = HCSR04US(US_5_trig, US_5_echo);
  Serial.print("US5 "); 
  Serial.print(cm5);
  Serial.print(",");
  


  //MQ2_2
  gas2 = analogRead(MQ2_2_sig);
  Serial.print("GAS2 ");
  Serial.print(gas2,DEC);
  Serial.print(",");

  //MQ2_3
  gas3 = analogRead(MQ2_3_sig);
  Serial.print("GAS3 ");
  Serial.print(gas3,DEC);
  Serial.print(",");
  
  //DHT11_5_sig
  DHT11(DHT11_5_sig, dht11_5_dat);
  Serial.print("Temp3 ");
//  Serial.print("Current humdity = ");
//  Serial.print(dht11_5_dat[0], DEC);
//  Serial.print(".");
//  Serial.print(dht11_5_dat[1], DEC);
//  Serial.print("%  ");
//  Serial.print("temperature = ");
  Serial.print(dht11_5_dat[2], DEC);
  Serial.print(".");
  Serial.print(dht11_5_dat[3], DEC);
  Serial.print(" ");
//  Serial.println("C  ");


  //DHT11_6_sig
  DHT11(DHT11_6_sig, dht11_6_dat);
//  Serial.print("DHT_11_6 ");
//  Serial.print("Current humdity = ");
//  Serial.print(dht11_6_dat[0], DEC);
//  Serial.print(".");
//  Serial.print(dht11_6_dat[1], DEC);
//  Serial.print("%  ");
//  Serial.print("temperature = ");
  Serial.print(dht11_6_dat[2], DEC);
  Serial.print(".");
  Serial.print(dht11_6_dat[3], DEC);
  Serial.print(",");
//  Serial.println("C  ");

  //DHT11_7_sig
  DHT11(DHT11_7_sig, dht11_7_dat);
  Serial.print("Temp4 ");
//  Serial.print("Current humdity = ");
//  Serial.print(dht11_7_dat[0], DEC);
//  Serial.print(".");
//  Serial.print(dht11_7_dat[1], DEC);
//  Serial.print("%  ");
//  Serial.print("temperature = ");
  Serial.print(dht11_7_dat[2], DEC);
  Serial.print(".");
  Serial.print(dht11_7_dat[3], DEC);
  Serial.print(" ");
//  Serial.println("C  ");



  //DHT11_8_sig
  DHT11(DHT11_8_sig, dht11_8_dat);
//  Serial.print("DHT_11_8 ");
//  Serial.print("Current humdity = ");
//  Serial.print(dht11_8_dat[0], DEC);
//  Serial.print(".");
//  Serial.print(dht11_8_dat[1], DEC);
//  Serial.print("%  ");
//  Serial.print("temperature = ");
  Serial.print(dht11_8_dat[2], DEC);
  Serial.print(".");
  Serial.print(dht11_8_dat[3], DEC);
  Serial.println();
//  Serial.println("C  ");

  delay(3000);
}

